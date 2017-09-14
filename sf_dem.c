#include "gimglib.h"

void dump_dem (struct subfile_struct *sf)
{
	struct garmin_dem *header = (struct garmin_dem *)sf->header;

	assert(sf->typeid == ST_DEM);

	struct garmin_dem_block3 *block3 = (struct garmin_dem_block3 *)(sf->base + header->points_to_block3);

	dump_comm(sf->header);

	printf("=== DEM HEADER ===\n");
	printf("Flags:             0x%x\n", header->flags);
	printf("Zoom Levels:       0x%x\n", header->zoom_levels);
	printf("Reserved:          0x%x\n", header->reserved0);
	printf("Record Size:       0x%x\n", header->record_size);
	printf("Points to Block 3: 0x%x\n", header->points_to_block3);
	printf("Reserved:          0x%x\n", header->reserved2);

	if (header->comm.hlen > sizeof(struct garmin_gmp))
		printf("from 0x%lx to 0x%x (0x%lx bytes): %s\n",
				sizeof(struct garmin_gmp), header->comm.hlen - 1,
				header->comm.hlen - sizeof(struct garmin_gmp),
				dump_unknown_bytes((uint8_t *)header + sizeof(struct garmin_gmp), header->comm.hlen - sizeof(struct garmin_gmp)));

	//block3
	printf("=== DEM BLOCK3 ===\n");
	printf("Record index:           %u\n", block3->record_index);
	printf("Number of pixel(X):     %u\n", block3->pixel_num_x);
	printf("Number of pixel(Y):     %u\n", block3->pixel_num_y);
	printf("Size of pixel(X):       %u\n", block3->pixel_size_x);
	printf("size of pixel(Y):       %u\n", block3->pixel_size_y);
	printf("unknown_12:             0x%x\n", block3->unknown_12);
	printf("Max Tile index(X):      %u\n", block3->tile_index_max_x);
	printf("Max Tile index(Y):      %u\n", block3->tile_index_max_y);
	printf("Block1 offset size:     %u\n", block3->block1_offset_size);
	printf("Block1 Base Hight size: %u\n", block3->block1_basehight_size);
	printf("Block1 Hight Diff size: %u\n", block3->block1_diffhight_size);
	printf("Block1 extra size:      %u\n", block3->block1_extra);
	printf("reserved1C:             0x%x\n", block3->reserved1C);
	printf("Tile size:              %u\n", block3->tile_size);
	printf("Points to Block 1:      0x%x\n", block3->points_to_block1);
	printf("Points to Block 2:      0x%x\n", block3->points_to_block2);
	printf("Boundary west:          %u\n", block3->boundary_west);
	printf("Boundary north:         %u\n", block3->boundary_north);
	printf("Pixel distance(X):      %u\n", block3->pixel_distance_x);
	printf("Pixel distance(Y):      %u\n", block3->pixel_distance_y);
	printf("Hight min:              0x%x\n", block3->hight_min);
	printf("Hight max:              0x%x\n", block3->hight_max);


	return;
}
