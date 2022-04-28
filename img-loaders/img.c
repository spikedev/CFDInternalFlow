#include "../opengl.h"
#include "../main.h"
#include "../project.h"
#include "jpeglib.h"
#include "jerror.h"

extern short (*LittleShort)(short var);
extern int (*LittleInt)(int var);
extern float (*LittleFloat)(float var);
extern short (*BigShort)(short var);
extern int (*BigInt)(int var);
extern float (*BigFloat)(float var);

void Img_VerticalFlip(texture_t *image)
{
	byte *tempdata;
	int pixel_bytes, i;

	pixel_bytes = image->bitdepth >> 3;

	if(!(tempdata = (byte *)malloc(image->width * image->height * pixel_bytes)))
		Sys_Error("Img_VerticalFlip: Failed to allocate memory for image");

	for(i = 0; i < image->height; i++)
		memcpy(&tempdata[i * image->width * pixel_bytes], &image->data[((image->height - 1) - i) * image->width * pixel_bytes], image->width * pixel_bytes);

	memcpy(image->data, tempdata, image->width * image->height * pixel_bytes);
}

#pragma pack(push, 1)

typedef struct
{
	unsigned short	signature;
	unsigned int	file_size;
	unsigned short	reserved[2];
	unsigned int	data_offset;

	unsigned int	headerinfo_size, width, height;
	unsigned short	image_planes, bitdepth;
	unsigned int	compression, image_size, horizontal_res;
	unsigned int	vertical_res, num_colors, num_significant_colors;
} bmp_header_t;

#pragma pack(pop)

void Img_LoadBMP(FILE *imgfile, const char *path, texture_t *image, bool alpha)
{
//	FILE *imgfile;
	bmp_header_t header;
	byte *palette = NULL;
	byte element;
	int pad, mem, remainder, bitdepth, pixel_bytes;
	int i, j, k;

//	if(!(imgfile = fopen(path, "rb")))
//		Sys_Error("Img_LoadBMP: Failed to open image file %s or empty file", path);

	//load the header
	if(fread(&header, 1, sizeof(bmp_header_t), imgfile) < sizeof(bmp_header_t))
	{
		fclose(imgfile);
		Sys_Error("Img_LoadBMP: Invalid image file %s", path);
	}

	if(LittleShort(header.signature) != 0x4d42)
	{
		fclose(imgfile);
		Sys_Error("Img_LoadBMP: Invalid bitmap image file %s", path);
	}

	image->width = LittleInt(header.width);
	image->height = LittleInt(header.height);
	bitdepth = (int)LittleShort(header.bitdepth);

	if((bitdepth != 24) && (bitdepth != 8) && (bitdepth != 4) && (bitdepth != 1))
	{
		fclose(imgfile);
		Sys_Error("Img_LoadBMP: Unsupported bitmap image color depth of %d in file %s", bitdepth, path);
	}

	if(bitdepth != 24)
	{
		int palette_entries;

		//load the color palette
		if(LittleInt(header.data_offset) <= 54)
		{
			fclose(imgfile);
			Sys_Error("Img_LoadBMP: %d-bit bitmap image file %s missing color palette", bitdepth, path);
		}

		palette_entries = 1 << bitdepth;

		if(!(palette = (byte *)calloc(palette_entries * 3,  sizeof(byte))))
		{
			fclose(imgfile);
			Sys_Error("Img_LoadBMP: Not enough memory to load %s", path);
		}

		for(i = 0; i < palette_entries; i++)
		{
			fread(&palette[i * 3 + 2], sizeof(byte), 1, imgfile);
			fread(&palette[i * 3 + 1], sizeof(byte), 1, imgfile);
			fread(&palette[i * 3], sizeof(byte), 1, imgfile);
			fread(&element, sizeof(byte), 1, imgfile);
		}
	}

	//load the image data
	if(alpha)
	{
		pixel_bytes = 4;
		image->bitdepth = 32;
	}
	else
	{
		pixel_bytes = 3;
		image->bitdepth = 24;
	}

	if(!(image->data = (byte *)calloc(image->width * image->height * pixel_bytes, sizeof(byte))))
	{
		fclose(imgfile);
		if(palette)
		{
			free(palette);
			palette = NULL;
		}

		Sys_Error("Img_LoadBMP: Not enough memory to load %s", path);
	}

	switch(bitdepth)
	{
		case 1:
			mem = image->width / 8;
			remainder = image->width % 8;
			if(remainder)
				mem++;

			pad = mem % 4;
			if(pad)
				pad = 4 - pad;

			i = 0;
			for(j = 0; j < image->height; j++)
			{
				for(k = 0; k < mem; k++)
				{
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i++] = palette[((element & 0x80) >> 7) * 3];
					image->data[i++] = palette[((element & 0x80) >> 7) * 3 + 1];
					image->data[i++] = palette[((element & 0x80) >> 7) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 1)
						break;

					image->data[i++] = palette[((element & 0x40) >> 6) * 3];
					image->data[i++] = palette[((element & 0x40) >> 6) * 3 + 1];
					image->data[i++] = palette[((element & 0x40) >> 6) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 2)
						break;

					image->data[i++] = palette[((element & 0x20) >> 5) * 3];
					image->data[i++] = palette[((element & 0x20) >> 5) * 3 + 1];
					image->data[i++] = palette[((element & 0x20) >> 5) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 3)
						break;

					image->data[i++] = palette[((element & 0x10) >> 4) * 3];
					image->data[i++] = palette[((element & 0x10) >> 4) * 3 + 1];
					image->data[i++] = palette[((element & 0x10) >> 4) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 4)
						break;

					image->data[i++] = palette[((element & 8) >> 3) * 3];
					image->data[i++] = palette[((element & 8) >> 3) * 3 + 1];
					image->data[i++] = palette[((element & 8) >> 3) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 5)
						break;

					image->data[i++] = palette[((element & 4) >> 2) * 3];
					image->data[i++] = palette[((element & 4) >> 2) * 3 + 1];
					image->data[i++] = palette[((element & 4) >> 2) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 6)
						break;

					image->data[i++] = palette[((element & 2) >> 1) * 3];
					image->data[i++] = palette[((element & 2) >> 1) * 3 + 1];
					image->data[i++] = palette[((element & 2) >> 1) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 7)
						break;

					image->data[i++] = palette[(element & 1) * 3];
					image->data[i++] = palette[(element & 1) * 3 + 1];
					image->data[i++] = palette[(element & 1) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;
				}

				//padding
				for(k = 0; k < pad; k++)
					fread(&element, sizeof(byte), 1, imgfile);
			}

			break;

		case 4:
			mem = image->width / 2;
			remainder = image->width % 2;
			if(remainder)
				mem++;

			pad = mem % 4;
			if(pad)
				pad = 4 - pad;

			i = 0;
			for(j = 0; j < image->height; j++)
			{
				for(k = 0; k < mem; k++)
				{
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i++] = palette[((element & 0xf0) >> 4) * 3];
					image->data[i++] = palette[((element & 0xf0) >> 4) * 3 + 1];
					image->data[i++] = palette[((element & 0xf0) >> 4) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;

					if(k == (mem - 1) && remainder == 1)
						break;

					image->data[i++] = palette[(element & 0xf) * 3];
					image->data[i++] = palette[(element & 0xf) * 3 + 1];
					image->data[i++] = palette[(element & 0xf) * 3 + 2];
					if(alpha)
						image->data[i++] = 255;
				}

				//padding
				for(k = 0; k < pad; k++)
					fread(&element, sizeof(byte), 1, imgfile);
			}

			break;

		case 8:
			pad = image->width % 4;
			if(pad)
				pad = 4 - pad;

			i = 0;
			for(j = 0; j < image->height; j++)
			{
				for(k = 0; k < image->width; k++)
				{
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i++] = palette[element * 3];
					image->data[i++] = palette[element * 3 + 1];
					image->data[i++] = palette[element * 3 + 2];
					if(alpha)
						image->data[i++] = 255;
				}

				//padding
				for(k = 0; k < pad; k++)
					fread(&element, sizeof(byte), 1, imgfile);
			}

			break;

		case 24:
			pad = (image->width * 3) % 4;
			if(pad)
				pad = 4 - pad;

			i = 0;
			for(j = 0; j < image->height; j++)
			{
				for(k = 0; k < image->width; k++)
				{
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i + 2] = element;
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i + 1] = element;
					fread(&element, sizeof(byte), 1, imgfile);
					image->data[i] = element;
					if(alpha)
						image->data[i + 3] = 255;
//FIXME: does this need to load the reserved slot? no padding needed for 24-bit?
					i += pixel_bytes;
				}

				//padding
				for(k = 0; k < pad; k++)
					fread(&element, sizeof(byte), 1, imgfile);
			}
	}

	if(palette)
	{
		free(palette);
		palette = NULL;
	}

//	fclose(imgfile);

//XXX: ?
//	Img_VerticalFlip(image);
}

void Img_WriteBMP(const char *path, texture_t *texture)
{
	FILE *fp;
	int i, j, sk, pad;
	int temp;

	if(!(fp = fopen(path, "wb")))
		Sys_Error("Img_WriteBMP: Failed to open file %s", path);

	//write the BMP tag
	fwrite("BM", 2, 1, fp);

	if(texture->bitdepth == 32)
		sk = 4;
	else
		sk = 3;

	pad = (texture->width * 3) % 4;
	if(pad)
		pad = 4 - pad;

	temp = 54 + texture->width * texture->height * 3 + (pad * texture->height);
	fwrite(&temp, sizeof(dword), 1, fp);			//file size
	temp = 0;
	fwrite(&temp, sizeof(word), 1, fp);				//reserved 1
	fwrite(&temp, sizeof(word), 1, fp);				//reserved 2
	temp = 54;
	fwrite(&temp, sizeof(dword), 1, fp);			//image data offset
	temp = 40;
	fwrite(&temp, sizeof(dword), 1, fp);			//header size
	fwrite(&texture->width, sizeof(int), 1, fp);	//width
	fwrite(&texture->height, sizeof(int), 1, fp);	//height
	temp = 1;
	fwrite(&temp, sizeof(word), 1, fp);				//number of image planes
	temp = 24;
	fwrite(&temp, sizeof(word), 1, fp);				//bits-per-pixel
	temp = 0;
	fwrite(&temp, sizeof(dword), 1, fp);			//compression method
	fwrite(&temp, sizeof(dword), 1, fp);			//size of bitmap
	fwrite(&temp, sizeof(int), 1, fp);				//horizontal resolution
	fwrite(&temp, sizeof(int), 1, fp);				//vertical resolution
	fwrite(&temp, sizeof(dword), 1, fp);			//number of colors used
	fwrite(&temp, sizeof(dword), 1, fp);			//number of significant colors

	temp = 0;
	for(i = 0; i < texture->height; i++)
	{
		for(j = 0; j < texture->width; j++)
		{
			fwrite(&texture->data[(i * texture->width + j) * sk + 2], sizeof(byte), 1, fp);
			fwrite(&texture->data[(i * texture->width + j) * sk + 1], sizeof(byte), 1, fp);
			fwrite(&texture->data[(i * texture->width + j) * sk], sizeof(byte), 1, fp);
		}

		for(j = 0; j < pad; j++)
			fwrite(&temp, sizeof(byte), 1, fp);
	}

	fclose(fp);
}

#pragma pack(push, 1)

typedef struct
{
	unsigned char	id_length, colormap_type, image_type;

	unsigned short	colormap_index, colormap_length;
	unsigned char	colormap_size;

	unsigned short	x_origin, y_origin, width, height;
	unsigned char	bitdepth, attributes;
} tga_header_t;

#pragma pack(pop)

void Img_LoadTGA(FILE *fp, const char *path, texture_t *image, bool alpha)
{
//	FILE *fp;
	tga_header_t header;
	byte *colormap = NULL;
	int colormap_bytes = 0, pixel_bytes, addy, tmp_col;
	int x, y, i, j;
	bool grayscale = false;

//	if(!(fp = fopen(path, "rb")))
//		Sys_Error("Img_LoadTGA: Failed to open image file %s or empty file", path);

	//load the header
	if(fread(&header, 1, sizeof(tga_header_t), fp) < sizeof(tga_header_t))
	{
		fclose(fp);
		Sys_Error("Img_LoadTGA: Invalid image file %s", path);
	}

	if(!header.image_type)
	{
		fclose(fp);
		Sys_Error("Img_LoadTGA: Image file %s is empty", path);
	}

	image->width = (int)LittleShort(header.width);
	image->height = (int)LittleShort(header.height);

	if((header.bitdepth != 32) && (header.bitdepth != 24) && (header.bitdepth != 8))
	{
		fclose(fp);
		Sys_Error("Img_LoadTGA: Unsupported targa image color depth of %d in file %s", header.bitdepth, path);
	}

	image->bitdepth = (int)header.bitdepth;

	if((header.bitdepth == 8) && !header.colormap_type && !((header.image_type == 3) || (header.image_type == 11)))
	{
		fclose(fp);
		Sys_Error("Img_LoadTGA: Colormap is missing in 8-bit targa image file %s", path);
	}

	if((header.image_type == 3) || (header.image_type == 11))
		grayscale = true;

	if((image->bitdepth == 8) && !header.colormap_type && !grayscale)
	{
		fclose(fp);
		Sys_Error("Img_LoadTGA: No colormap for 8-bit image file %s", path);
	}

	//skip over the image ID field
	fseek(fp, header.id_length, SEEK_CUR);

	if(header.colormap_type)
	{
		int colormap_entries;

		//load the colormap
		if((LittleShort(header.colormap_size) != 32) && (LittleShort(header.colormap_size) != 24))
		{
			fclose(fp);
			Sys_Error("Img_LoadTGA: Unsupported colormap color depth of %d in targa image file %s", LittleShort(header.colormap_size), path);
		}

		colormap_bytes = LittleShort(header.colormap_size) / 8;
		colormap_entries = 1 << header.bitdepth;

		if(!(colormap = (byte *)calloc(colormap_entries * colormap_bytes, sizeof(byte))))
		{
			fclose(fp);
			Sys_Error("Img_LoadTGA: Failed to allocate memory for the colormap; %s", path);
		}

		if((int)fread(colormap, 1, colormap_entries * colormap_bytes * sizeof(byte), fp) < colormap_entries * colormap_bytes)
		{
			fclose(fp);
			if(colormap)
			{
				free(colormap);
				colormap = NULL;
			}

			Sys_Error("Img_LoadTGA: Invalid image file %s", path);
		}
	}

	//load the image data
	if(alpha)
		pixel_bytes = 4;
	else
		pixel_bytes = 3;

	if(!(image->data = (byte *)calloc(image->width * image->height * pixel_bytes, sizeof(byte))))
	{
		fclose(fp);
		if(colormap)
		{
			free(colormap);
			colormap = NULL;
		}

		Sys_Error("Img_LoadTGA: Failed to allocate memory to load %s", path);
	}

	if(header.image_type < 9)
	{
		//uncompressed
		for(i = 0; i < image->width * image->height; i++)
		{
			tmp_col = Img_TGAGetPixel(grayscale, colormap, colormap_bytes, header.bitdepth, fp);

			switch((header.attributes & 0x30) >> 4)
			{
				case 1:				//lower right
					x = image->width - 1 - (i % image->width);
					y = i / image->height;
					break;

				case 2:				//upper left
					x = i % image->width;
					y = image->height - 1 - (i / image->width);
					break;

				case 3:				//upper right
					x = image->width - 1 - (i % image->width);
					y = image->height - 1 - (i / image->width);
					break;

				case 0:				//lower left
				default:
					x = i % image->width;
					y = i / image->width;
			}

			addy = (y * image->width + x) * pixel_bytes;
			for(j = 0; j < pixel_bytes; j++)
				image->data[addy + j] = (byte)((tmp_col >> (j * 8)) & 0xff);
		}
	}
	else
	{
		int repcount, k;
		byte packet_header;

		//RLE compression
		for(i = 0; i < image->width * image->height; i++)
		{
			fread(&packet_header, sizeof(byte), 1, fp);
			if(packet_header & 0x80)
			{
				//run-length packet
				tmp_col = Img_TGAGetPixel(grayscale, colormap, colormap_bytes, header.bitdepth, fp);

				repcount = (packet_header & 0x7f) + 1;
				for(j = 0; j < repcount; j++)
				{
					switch((header.attributes & 0x30) >> 4)
					{
						case 1:				//lower right
							x = image->width - 1 - ((i + j) % image->width);
							y = (i + j) / image->height;
							break;

						case 2:				//upper left
							x = (i + j) % image->width;
							y = image->height - 1 - ((i + j) / image->width);
							break;

						case 3:				//upper right
							x = image->width - 1 - ((i + j) % image->width);
							y = image->height - 1 - ((i + j) / image->width);
							break;

						case 0:				//lower left
						default:
							x = (i + j) % image->width;
							y = (i + j) / image->width;
					}

					addy = (y * image->width + x) * pixel_bytes;
					for(k = 0; k < pixel_bytes; k++)
						image->data[addy + k] = (byte)((tmp_col >> (k * 8)) & 0xff);
				}

				i += (repcount - 1);
			}
			else
			{
				//raw data packet
				repcount = (packet_header & 0x7f) + 1;
				for(j = 0; j < repcount; j++)
				{
					tmp_col = Img_TGAGetPixel(grayscale, colormap, colormap_bytes, header.bitdepth, fp);

					switch((header.attributes & 0x30) >> 4)
					{
						case 1:				//lower right
							x = image->width - 1 - ((i + j) % image->width);
							y = (i + j) / image->height;
							break;

						case 2:				//upper left
							x = (i + j) % image->width;
							y = image->height - 1 - ((i + j) / image->width);
							break;

						case 3:				//upper right
							x = image->width - 1 - ((i + j) % image->width);
							y = image->height - 1 - ((i + j) / image->width);
							break;

						case 0:				//lower left
						default:
							x = (i + j) % image->width;
							y = (i + j) / image->width;
					}

					addy = (y * image->width + x) * pixel_bytes;
					for(k = 0; k < pixel_bytes; k++)
						image->data[addy + k] = (byte)((tmp_col >> (k * 8)) & 0xff);
				}

				i += (repcount - 1);
			}
		}
	}

	if(colormap)
	{
		free(colormap);
		colormap = NULL;
	}

//	fclose(fp);

	if(alpha)
		image->bitdepth = 32;
	else
		image->bitdepth = 24;
//XXX: ?
//	Img_VerticalFlip(image);
}

int Img_TGAGetPixel(bool gray_scale, byte *colormap, int colormap_bytes, int bitdepth, FILE *fp)
{
	byte r, g, b, a;

	switch(bitdepth)
	{
		case 8:
			if(!gray_scale)
			{
				byte temp;

				fread(&temp, sizeof(byte), 1, fp);
				b = colormap[temp * colormap_bytes];
				g = colormap[temp * colormap_bytes + 1];
				r = colormap[temp * colormap_bytes + 2];
				if(colormap_bytes > 3)
					a = colormap[temp * colormap_bytes + 3];
				else
					a = 255;
			}
			else
			{
				fread(&r, sizeof(byte), 1, fp);
				g = b = r;
				a = 255;
			}

			break;

		case 24:
			fread(&b, sizeof(byte), 1, fp);
			fread(&g, sizeof(byte), 1, fp);
			fread(&r, sizeof(byte), 1, fp);
			a = 255;
			break;

		case 32:
			fread(&b, sizeof(byte), 1, fp);
			fread(&g, sizeof(byte), 1, fp);
			fread(&r, sizeof(byte), 1, fp);
			fread(&a, sizeof(byte), 1, fp);
			break;
	}

	return r + (g << 8) + (b << 16) + (a << 24);
}

void Img_LoadJPEG(FILE *fp, const char *path, texture_t *image, bool alpha)
{
	struct jpeg_decompress_struct cinfo;
	struct jpeg_error_mgr jerr;
	int bitdepth, pixel_bytes, bpb;
	int i, j;
	byte *dptr, *dbuffer;
//	FILE *fp;

//	if(!(fp = fopen(path, "rb")))
//		Sys_Error("Img_LoadTGA: Failed to open image file %s or empty file", path);

	cinfo.err = jpeg_std_error(&jerr);
	jpeg_create_decompress(&cinfo);
	jpeg_stdio_src(&cinfo, fp);
	jpeg_read_header(&cinfo, true);
	cinfo.out_color_space = JCS_RGB;
	jpeg_start_decompress(&cinfo);

	image->width = cinfo.output_width;
	image->height = cinfo.output_height;
	bitdepth = cinfo.output_components * 8;
	bpb = cinfo.output_components;

	//TODO: ?
	if(bitdepth < 24)
		Sys_Error("Img_LoadJPEG: Unsupported jpg file %s", path);

	//load the image data
	if(alpha)
	{
		pixel_bytes = 4;
		image->bitdepth = 32;
	}
	else
	{
		pixel_bytes = 3;
		image->bitdepth = 24;
	}

	if(!(image->data = (byte *)calloc(image->width * image->height * pixel_bytes, sizeof(byte))))
	{
		fclose(fp);
		Sys_Error("Img_LoadJPEG: Not enough memory to load %s", path);
	}

	if(!(dbuffer = (byte *)malloc(image->width * bpb)))
	{
		fclose(fp);
		Sys_Error("Img_LoadJPEG: Not enough memory to load %s", path);
	}

	j = 0;
	while(cinfo.output_scanline < cinfo.output_height)
	{
		dptr = dbuffer;
		jpeg_read_scanlines(&cinfo, &dptr, 1);
		for(i = 0; i < image->width; i++)
		{
			image->data[j * image->width * pixel_bytes + (i * pixel_bytes)] = dbuffer[i * bpb];
			image->data[j * image->width * pixel_bytes + (i * pixel_bytes) + 1] = dbuffer[i * bpb + 1];
			image->data[j * image->width * pixel_bytes + (i * pixel_bytes) + 2] = dbuffer[i * bpb + 2];
			//TODO: jpg support alpha channel?
			if(alpha)
				image->data[j * image->width * pixel_bytes + (i * pixel_bytes) + 3] = 255;
		}

		j++;
	}

	jpeg_finish_decompress(&cinfo);

	jpeg_destroy_decompress(&cinfo);

	free(dbuffer);

	Img_VerticalFlip(image);

//	fclose(fp);
}
