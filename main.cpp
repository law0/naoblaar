#include "blc_image.h" //This include blc_core.h

int main(int argc, char **argv)
{

	blc_array image;
	int i,j, bytes_per_pixel, width, height;
	char const *input_filename="blc_image/lena512.png";
	char const *output_filename="blc_image/t_load_and_save_png/result.png";

	fprintf(stderr, "\nLoad the png file passed as argument ( '%s' by default ).\nAdd a red cross in the middle and save the result in '%s'.\n", input_filename, output_filename);

	if (argc==2) input_filename = argv[1];
	else if (argc > 2) EXIT_ON_ERROR("You can only have an image as argument. But you have '%d' arguments.", argc-1);

	//Define the array suitable for the imag in 'input_filenmae'. Does not allocate the memory.
	//You can use these options:
	//type : 'NDEF', 'UIN8', 'INT8', 'UI16', 'IN16'
	//format: 'NDEF', 'Y800' for gray, 'RGB3', 'RGBA'
	//A conversion may be done. If you use 'NDEF', it will be the native type and format of the image.
	blc_image_def_with_png_file(&image, input_filename, 'UIN8', 'RGB3');

	//We allocate the array memory
	image.allocate();

	//Update the content of the image with the png.
	//You can use it without the previous steps as long as the images of the png files are of the same size.
	//It is not a problem if they have a different format, a conversion will be done at each step.
	blc_image_update_with_png_file(&image, input_filename);

	// *** Use image as an array of .chars, .uchars or .ints16, .uints16 or .ints32, or uints32 ***
	bytes_per_pixel=blc_image_get_bytes_per_pixel(&image);
	//Depending on the format, the dims[0].length is the width ('Y800') or the bytes_per_pixel( i.e. 3 for 'RGB3')
	//In order to be sure for any format, we compare with the number of dims.
	width=image.dims[image.dims_nb-2].length;
	height=image.dims[image.dims_nb-1].length;

	//As an exemple, draw a red cross in the middle of the image.
	j=height/2;
	for(i=width/4; i!=width*3/4; i++)
	{
	    image.uchars[bytes_per_pixel*(i+width*j)]=255;
	}

	i=width/2;
	for(j=height/4; j!=height*3/4; j++)
	{
	    image.uchars[bytes_per_pixel*(i+width*j)]=255;
	}

	//Save the data in the blc_array 'image' as a png. It will be with the type and format of tha array
	blc_image_save_png_file(&image, output_filename);

	fprintf(stderr, "\nThe result has been saved in '%s'\n\n", output_filename);

	return 0;
}
