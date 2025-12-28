// SPDX-License-Identifier: Zlib
// Copyright (c) 2023-2025 Julien Bernard

#include <gf2/core/Graph.h>

#include <cassert>
#include <cstdint>

#include <algorithm>
#include <limits>
#include <stdexcept>

#include <gf2/core/BinaryHeap.h>

namespace gf {

  Graph::Graph(std::size_t n)
  {
    if (n > 0) {
      m_vertices.reserve(n);
      m_in_edges.reserve(n);
      m_out_edges.reserve(n);
    }
  }

  VertexId Graph::add_vertex()
  {
    auto id = VertexId{ m_next_vertex_id++ };
    m_vertices.push_back({ id });
    m_in_edges.emplace_back();
    m_out_edges.emplace_back();
    ++m_vertex_count;
    return id;
  }

  VertexRange Graph::vertices() const
  {
    return { m_vertices.size() };
  }

  std::size_t Graph::vertex_count() const
  {
    return m_vertex_count;
  }

  bool Graph::is_valid(VertexId v) const
  {
    return m_vertices[to_index(v)].id != NoVertex;
  }

  void Graph::remove_vertex(VertexId v)
  {
    assert(is_valid(v));
    m_vertices[to_index(v)].id = NoVertex;

    for (auto id : m_in_edges[to_index(v)]) {
      erase_edge(m_edges[to_index(id)]);
    }

    m_in_edges[to_index(v)].clear();

    for (auto id : m_out_edges[to_index(v)]) {
      erase_edge(m_edges[to_index(id)]);
    }

    m_out_edges[to_index(v)].clear();
  }

  EdgeId Graph::add_edge(VertexId source, VertexId target)
  {
    auto id = EdgeId{ m_next_edge_id++ };
    m_edges.push_back({ id, source, target });
    m_in_edges[to_index(target)].insert(id);
    m_out_edges[to_index(source)].insert(id);
    ++m_edge_count;
    return id;
  }

  EdgeRange Graph::edges() const
  {
    return { m_edges.size() };
  }

  std::size_t Graph::edge_count() const
  {
    return m_edge_count;
  }

  bool Graph::is_valid(EdgeId e) const
  {
    return m_edges[to_index(e)].id != NoEdge;
  }

  void Graph::remove_edge(EdgeId e)
  {
    assert(is_valid(e));
    [[maybe_unused]] std::size_t erased = 0;
    Edge& edge = m_edges[to_index(e)];
    erased = m_in_edges[to_index(edge.target)].erase(e);
    assert(erased == 1);
    erased = m_out_edges[to_index(edge.source)].erase(e);
    assert(erased == 1);
    erase_edge(edge);
  }

  VertexId Graph::source(EdgeId e) const
  {
    return m_edges[to_index(e)].source;
  }

  VertexId Graph::target(EdgeId e) const
  {
    return m_edges[to_index(e)].target;
  }

  auto Graph::in_edges(VertexId v) const -> InEdgeRange
  {
    assert(to_index(v) < m_in_edges.size());
    const std::set<EdgeId>& edges = m_in_edges[to_index(v)];
    return { edges.begin(), edges.end() };
  }

  auto Graph::out_edges(VertexId v) const -> OutEdgeRange
  {
    assert(to_index(v) < m_out_edges.size());
    const std::set<EdgeId>& edges = m_out_edges[to_index(v)];
    return { edges.begin(), edges.end() };
  }

  void Graph::clear()
  {
    m_next_vertex_id = 0;
    m_next_edge_id = 0;
    m_vertices.clear();
    m_edges.clear();
    m_in_edges.clear();
    m_out_edges.clear();
  }

  void Graph::erase_edge(Edge& edge)
  {
    if (edge.id == NoEdge) {
      return;
    }

    edge.id = NoEdge;
    edge.source = NoVertex;
    edge.target = NoVertex;
    --m_edge_count;
  }

  /*
   * topological_sort
   */

  namespace {

    class TopologicalSort {
    public:
      TopologicalSort(const Graph& graph)
      : m_graph(graph)
      {
      }

      std::vector<VertexId> operator()()
      {
        const VertexRange range = m_graph.vertices();
        m_color.clear();
        m_color.resize(range.size(), Color::White);

        try {

          std::vector<VertexId> vertices;

          for (auto vertex : range) {
            if (m_graph.is_valid(vertex) && m_color[to_index(vertex)] != Color::Black) {
              visit(vertex, vertices);
            }
          }

          std::ranges::reverse(vertices);
          return vertices;

        } catch ([[maybe_unused]] std::exception& ex) {
          return {};
        }

        return {};
      }

    private:
      enum class Color : uint8_t {
        White,
        Gray,
        Black,
      };

      // NOLINTNEXTLINE(misc-no-recursion)
      void visit(VertexId vertex, std::vector<VertexId>& vertices) {
        assert(m_graph.is_valid(vertex));

        Color& color = m_color[to_index(vertex)];

        if (color == Color::Black) {
          return;
        }

        if (color == Color::Gray) {
          // a cycle has been detected
          throw std::runtime_error("Not a DAG");
        }

        color = Color::Gray;

        for (auto edge : m_graph.out_edges(vertex)) {
          visit(m_graph.target(edge), vertices);
        }

        color = Color::Black;
        vertices.push_back(vertex);
      }

      const Graph& m_graph; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
      std::vector<Color> m_color;
    };

  }


  std::vector<VertexId> topological_sort(const Graph& graph)
  {
    TopologicalSort sort(graph);
    return sort();
  }


  /*
   * compute_shortest_path
   */

  namespace {
    struct GraphDijkstraHeapData {
      VertexId vertex = NoVertex;
      double distance = 0.0;
    };

    bool operator<(const GraphDijkstraHeapData& lhs, const GraphDijkstraHeapData& rhs)
    {
      return lhs.distance > rhs.distance;
    }

    using GraphDijkstraHeap = BinaryHeap<GraphDijkstraHeapData>;

    class GraphDijkstraAlgorithm {
    public:
      GraphDijkstraAlgorithm(const Graph& graph)
      : m_graph(graph)
      , m_handles(graph.vertex_count())
      {
      }

      std::vector<GraphShortestPath> operator()(VertexId origin, GraphRouteCostFunction& function)
      {
        std::vector<GraphShortestPath> paths(m_graph.vertex_count(), { NoVertex, std::numeric_limits<double>::infinity() });
        initialize(origin, paths);

        while (!m_heap.empty()) {
          const GraphDijkstraHeapData data = m_heap.top();
          m_heap.pop();
          compute_vertex(data, function, paths);
        }

        return paths;
      }

    private:
      void initialize(VertexId origin, std::vector<GraphShortestPath>& paths)
      {
        paths[to_index(origin)].distance = 0.0;

        for (const VertexId vertex : m_graph.vertices()) {
          GraphDijkstraHeapData data = {};
          data.vertex = vertex;
          data.distance = paths[to_index(vertex)].distance;
          m_handles[to_index(vertex)] = m_heap.push(data);
        }
      }

      void compute_vertex(GraphDijkstraHeapData heap_data, GraphRouteCostFunction& function, std::vector<GraphShortestPath>& paths)
      {
        for (const EdgeId edge : m_graph.out_edges(heap_data.vertex)) {
          const double updated_distance = paths[to_index(heap_data.vertex)].distance + function(edge);
          const VertexId target = m_graph.target(edge);

          if (updated_distance < paths[to_index(target)].distance) {
            auto& path_data = paths[to_index(target)];
            path_data.distance = updated_distance;
            path_data.previous = heap_data.vertex;

            assert(m_heap(m_handles[to_index(target)]).vertex == target);
            m_heap(m_handles[to_index(target)]).distance = updated_distance;
            m_heap.increase(m_handles[to_index(target)]);
          }
        }
      }

      const Graph& m_graph; // NOLINT(cppcoreguidelines-avoid-const-or-ref-data-members)
      GraphDijkstraHeap m_heap;
      std::vector<GraphDijkstraHeap::handle_type> m_handles;
    };


  }

  std::vector<GraphShortestPath> compute_shortest_path(const Graph& graph, VertexId origin, GraphRouteCostFunction function)
  {
    GraphDijkstraAlgorithm algorithm(graph);
    return algorithm(origin, function);
  }

}
