#!/bin/sh

WORKING_DIR="output"
OUTPUT_DIR="../assets/fig"

rm -rf "${WORKING_DIR}"
mkdir "${WORKING_DIR}"

rm -rf "${OUTPUT_DIR}"
mkdir "${OUTPUT_DIR}"

for I in *.png
do
  convert -trim ${I} "${WORKING_DIR}/${I}"
  rm -f "$I"
done

# orthogonal

mv "${WORKING_DIR}/grids-01.png" "${OUTPUT_DIR}/orthogonal_tile.png"
mv "${WORKING_DIR}/grids-02.png" "${OUTPUT_DIR}/orthogonal_layout.png"
mv "${WORKING_DIR}/grids-03.png" "${OUTPUT_DIR}/orthogonal_neighbors.png"

# isometric

mv "${WORKING_DIR}/grids-04.png" "${OUTPUT_DIR}/isometric_tile.png"
mv "${WORKING_DIR}/grids-05.png" "${OUTPUT_DIR}/isometric_layout.png"
mv "${WORKING_DIR}/grids-06.png" "${OUTPUT_DIR}/isometric_neighbors.png"

# staggered

mv "${WORKING_DIR}/grids-07.png" "${OUTPUT_DIR}/staggered_tile.png"
mv "${WORKING_DIR}/grids-08.png" "${OUTPUT_DIR}/staggered_x_even_layout.png"
mv "${WORKING_DIR}/grids-09.png" "${OUTPUT_DIR}/staggered_x_odd_layout.png"
mv "${WORKING_DIR}/grids-10.png" "${OUTPUT_DIR}/staggered_y_even_layout.png"
mv "${WORKING_DIR}/grids-11.png" "${OUTPUT_DIR}/staggered_y_odd_layout.png"
mv "${WORKING_DIR}/grids-12.png" "${OUTPUT_DIR}/staggered_rx_ry.png"
mv "${WORKING_DIR}/grids-13.png" "${OUTPUT_DIR}/staggered_x_even_coordinates.png"
mv "${WORKING_DIR}/grids-14.png" "${OUTPUT_DIR}/staggered_x_odd_coordinates.png"
mv "${WORKING_DIR}/grids-15.png" "${OUTPUT_DIR}/staggered_y_even_coordinates.png"
mv "${WORKING_DIR}/grids-16.png" "${OUTPUT_DIR}/staggered_y_odd_coordinates.png"
mv "${WORKING_DIR}/grids-17.png" "${OUTPUT_DIR}/staggered_x_1_neighbors.png"
mv "${WORKING_DIR}/grids-18.png" "${OUTPUT_DIR}/staggered_x_0_neighbors.png"
mv "${WORKING_DIR}/grids-19.png" "${OUTPUT_DIR}/staggered_y_1_neighbors.png"
mv "${WORKING_DIR}/grids-20.png" "${OUTPUT_DIR}/staggered_y_0_neighbors.png"

# hexagonal

mv "${WORKING_DIR}/grids-21.png" "${OUTPUT_DIR}/hexagonal_x_tile.png"
mv "${WORKING_DIR}/grids-22.png" "${OUTPUT_DIR}/hexagonal_y_tile.png"
mv "${WORKING_DIR}/grids-23.png" "${OUTPUT_DIR}/hexagonal_x_even_layout.png"
mv "${WORKING_DIR}/grids-24.png" "${OUTPUT_DIR}/hexagonal_x_odd_layout.png"
mv "${WORKING_DIR}/grids-25.png" "${OUTPUT_DIR}/hexagonal_y_even_layout.png"
mv "${WORKING_DIR}/grids-26.png" "${OUTPUT_DIR}/hexagonal_y_odd_layout.png"
mv "${WORKING_DIR}/grids-27.png" "${OUTPUT_DIR}/hexagonal_x_even_coordinates.png"
mv "${WORKING_DIR}/grids-28.png" "${OUTPUT_DIR}/hexagonal_x_odd_coordinates.png"
mv "${WORKING_DIR}/grids-29.png" "${OUTPUT_DIR}/hexagonal_y_even_coordinates.png"
mv "${WORKING_DIR}/grids-30.png" "${OUTPUT_DIR}/hexagonal_y_odd_coordinates.png"

