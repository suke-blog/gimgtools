#include "gimglib.h"

double get_degrees(uint32_t value)
{
	return (double)value * 0.00000008381903171539306640625;
}

void dump_dem_block1 (uint8_t *ptrBlock1, uint8_t offset_size,
	uint8_t basehight_size, uint8_t diffhight_size,
	uint8_t extra_size, uint8_t tile_count)
{
	uint8_t *ptr = ptrBlock1;

	for(int j=0; j<tile_count; j++){
		uint32_t offset_block2 = 0;
		uint16_t basehight = 0;
		uint16_t diffhight_max = 0;
		uint8_t encoding_type = 0;

		char *str = dump_unknown_bytes(ptr, offset_size+1);
		printf("offset:%s\n", str);
		for(int i=0; i<offset_size+1; i++){
			offset_block2 |= *(ptr++) << (8*i);
		}

		for(int i=0; i<basehight_size+1; i++){
			basehight |= *(ptr++) << (8*i);
		}

		for(int i=0; i<diffhight_size+1; i++){
			diffhight_max |= *(ptr++) << (8*i);
		}

		if(extra_size == 1)
			encoding_type = *(ptr++);

		printf("---BLOCK1 (%d / %d)---\n", j+1, tile_count);
		printf("> Offset Block2:  0x%x\n", offset_block2);
		printf("> Base Hight:     %u\n", basehight);
		printf("> Diff Hight Max: %u\n", diffhight_max);
		printf("> Encoding type:  %u\n", encoding_type);
	}
}

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
	for(int i=0; i<header->zoom_levels; i++){
		printf("=== DEM BLOCK3 (%d / %d) ===\n", i+1, header->zoom_levels);
		printf("Record index:           0x%x\n", block3[i].record_index);
		printf("Number of pixel(X):     %u\n", block3[i].pixel_num_x);
		printf("Number of pixel(Y):     %u\n", block3[i].pixel_num_y);
		printf("Size of pixel(X):       %u\n", block3[i].pixel_size_x);
		printf("size of pixel(Y):       %u\n", block3[i].pixel_size_y);
		printf("unknown_12:             0x%x\n", block3[i].unknown_12);
		printf("Max Tile index(X):      %u\n", block3[i].tile_index_max_x);
		printf("Max Tile index(Y):      %u\n", block3[i].tile_index_max_y);
		printf("Block1 offset size:     %u\n", block3[i].block1_offset_size);
		printf("Block1 Base Hight size: %u\n", block3[i].block1_basehight_size);
		printf("Block1 Hight Diff size: %u\n", block3[i].block1_diffhight_size);
		printf("Block1 extra size:      %u\n", block3[i].block1_extra);
		printf("reserved1C:             0x%x\n", block3[i].reserved1C);
		printf("Tile size:              %u\n", block3[i].tile_size);
		printf("Points to Block 1:      0x%x\n", block3[i].points_to_block1);
		printf("Points to Block 2:      0x%x\n", block3[i].points_to_block2);
		printf("Boundary west:          %f(0x%x)\n",
			get_degrees(block3[i].boundary_west),
			block3[i].boundary_west);
		printf("Boundary north:         %f(0x%x)\n",
			get_degrees(block3[i].boundary_north),
			block3[i].boundary_north);
		printf("Pixel distance(X):      %f(0x%x)\n",
			get_degrees(block3[i].pixel_distance_x),
			block3[i].pixel_distance_x);
		printf("Pixel distance(Y):      %f(0x%x)\n",
			get_degrees(block3[i].pixel_distance_y),
			block3[i].pixel_distance_y);
		printf("Hight min:              %u\n", block3[i].hight_min);
		printf("Hight max:              %u\n", block3[i].hight_max);

		dump_dem_block1((uint8_t *)(sf->base + block3[i].points_to_block1),
			block3[i].block1_offset_size,
			block3[i].block1_basehight_size,
			block3[i].block1_diffhight_size,
			block3[i].block1_extra,
			block3[i].tile_size);

		printf("\n-----------\n");
	}

	return;
}

