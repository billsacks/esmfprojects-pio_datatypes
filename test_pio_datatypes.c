#include <pio.h>

#include <mpi.h>

#include <cstring>
#include <iostream>
using namespace std;

int main(void) {
#define DIM_X 3
   signed char data_byte[DIM_X] = {-64, -32, 28}; // PIO_BYTE
   char data_char[DIM_X] = {65, 67, 69};
   short data_short[DIM_X] = {-2000, -1000, 3000};
   int data_int[DIM_X] = {-200000, -100000, 300000};
   float data_float[DIM_X] = {-3.5, -2.5, 1.5};
   double data_double[DIM_X] = {-7.5, -4.5, 8.5};

   signed char data_byte_read[DIM_X] = {-64, -32, 28}; // PIO_BYTE
   char data_char_read[DIM_X] = {65, 67, 69};
   short data_short_read[DIM_X] = {-2000, -1000, 3000};
   int data_int_read[DIM_X] = {-200000, -100000, 300000};
   float data_float_read[DIM_X] = {-3.5, -2.5, 1.5};
   double data_double_read[DIM_X] = {-7.5, -4.5, 8.5};

   // Skipping PIO_UBYTE, PIO_USHORT, PIO_UINT, PIO_INT64, PIO_UINT64 because they give errors about not a valid data type

   int iodesc_byte;
   int iodesc_char;
   int iodesc_short;
   int iodesc_int;
   int iodesc_float;
   int iodesc_double;

   int vardesc_byte;
   int vardesc_char;
   int vardesc_short;
   int vardesc_int;
   int vardesc_float;
   int vardesc_double;

   int rc;
   int iosys_handle;
   int pio_file;
   int dims[1];
   MPI_Offset compdof[DIM_X];
   int dimid_x;

   int num_iotasks = 1;
   int stride = 1;
   int base = 0;
   int rearr = PIO_REARR_SUBSET;
   rc = PIOc_Init_Intracomm(MPI_COMM_WORLD, num_iotasks, stride, base, rearr, &iosys_handle);

   int iotype = PIO_IOTYPE_NETCDF;
   string fname = "testfile.nc";
   int amode_in = PIO_CLOBBER;
   rc = PIOc_createfile(iosys_handle, &pio_file, &iotype, fname.c_str(), amode_in);

   dims[0] = DIM_X;
   for (int i=0; i<DIM_X; i++) {
      compdof[i] = i+1;
   }
   PIOc_InitDecomp(iosys_handle, PIO_BYTE, 1, dims, DIM_X, compdof, &iodesc_byte, NULL, NULL, NULL);
   PIOc_InitDecomp(iosys_handle, PIO_CHAR, 1, dims, DIM_X, compdof, &iodesc_char, NULL, NULL, NULL);
   PIOc_InitDecomp(iosys_handle, PIO_SHORT, 1, dims, DIM_X, compdof, &iodesc_short, NULL, NULL, NULL);
   PIOc_InitDecomp(iosys_handle, PIO_INT, 1, dims, DIM_X, compdof, &iodesc_int, NULL, NULL, NULL);
   PIOc_InitDecomp(iosys_handle, PIO_FLOAT, 1, dims, DIM_X, compdof, &iodesc_float, NULL, NULL, NULL);
   PIOc_InitDecomp(iosys_handle, PIO_DOUBLE, 1, dims, DIM_X, compdof, &iodesc_double, NULL, NULL, NULL);

   rc = PIOc_def_dim(pio_file, "x", dims[0], &dimid_x);

   rc = PIOc_def_var(pio_file, "data_byte", PIO_BYTE, 1, &dimid_x, &vardesc_byte);
   rc = PIOc_def_var(pio_file, "data_char", PIO_CHAR, 1, &dimid_x, &vardesc_char);
   rc = PIOc_def_var(pio_file, "data_short", PIO_SHORT, 1, &dimid_x, &vardesc_short);
   rc = PIOc_def_var(pio_file, "data_int", PIO_INT, 1, &dimid_x, &vardesc_int);
   rc = PIOc_def_var(pio_file, "data_float", PIO_FLOAT, 1, &dimid_x, &vardesc_float);
   rc = PIOc_def_var(pio_file, "data_double", PIO_DOUBLE, 1, &dimid_x, &vardesc_double);

   rc = PIOc_enddef(pio_file);

   rc = PIOc_write_darray(pio_file, vardesc_byte, iodesc_byte, DIM_X, data_byte, NULL);
   rc = PIOc_write_darray(pio_file, vardesc_char, iodesc_char, DIM_X, data_char, NULL);
   rc = PIOc_write_darray(pio_file, vardesc_short, iodesc_short, DIM_X, data_short, NULL);
   rc = PIOc_write_darray(pio_file, vardesc_int, iodesc_int, DIM_X, data_int, NULL);
   rc = PIOc_write_darray(pio_file, vardesc_float, iodesc_float, DIM_X, data_float, NULL);
   rc = PIOc_write_darray(pio_file, vardesc_double, iodesc_double, DIM_X, data_double, NULL);

   rc = PIOc_closefile(pio_file);

   rc = PIOc_openfile(iosys_handle, &pio_file, &iotype, fname.c_str(), 0);

   rc = PIOc_read_darray(pio_file, vardesc_byte, iodesc_byte, DIM_X, data_byte_read);
   rc = PIOc_read_darray(pio_file, vardesc_char, iodesc_char, DIM_X, data_char_read);
   rc = PIOc_read_darray(pio_file, vardesc_short, iodesc_short, DIM_X, data_short_read);
   rc = PIOc_read_darray(pio_file, vardesc_int, iodesc_int, DIM_X, data_int_read);
   rc = PIOc_read_darray(pio_file, vardesc_float, iodesc_float, DIM_X, data_float_read);
   rc = PIOc_read_darray(pio_file, vardesc_double, iodesc_double, DIM_X, data_double_read);

   printf("Byte: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%d ", (int)data_byte_read[i]);
      if (data_byte_read[i] != data_byte[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");

   printf("Char: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%c ", data_char_read[i]);
      if (data_char_read[i] != data_char[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");

   printf("Short: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%d ", data_short_read[i]);
      if (data_short_read[i] != data_short[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");

   printf("Int: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%d ", data_int_read[i]);
      if (data_int_read[i] != data_int[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");

   printf("Float: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%.2f ", data_float_read[i]);
      if (data_float_read[i] != data_float[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");

   printf("Double: ");
   for (int i=0; i < DIM_X; i++) {
      printf("%.2f ", data_double_read[i]);
      if (data_double_read[i] != data_double[i]) {
         printf("DIFFERS! ");
      }
   }
   printf("\n");
}
