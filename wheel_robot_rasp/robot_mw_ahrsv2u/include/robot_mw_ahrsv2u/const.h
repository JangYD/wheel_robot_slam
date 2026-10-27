#ifndef CONST_H
#define CONST_H
#define M_PI 3.14159265358979323846
//ASCII
//const unsigned char angle_cmd[5] = {0X61, 0X6E, 0X67, 0X0D, 0X0A}; //ang enter
//const unsigned char acc_cmd[5] = {0X61, 0X63, 0X63, 0X0D, 0X0A}; //acc enter
//const unsigned char gyr_cmd[5] = {0x67, 0x79, 0x72, 0X0D, 0X0A}; //gyr enter
//const unsigned char reset_angle_cmd[5] = {0x7A, 0x72, 0x6F, 0X0D, 0X0A}; //zro enter
//const unsigned char reset_cmd[5] = {0x7A, 0x72, 0x6F, 0X0D, 0X0A};
//const unsigned char av_cmd[7] = {0x61, 0x76, 0x3D, 0x31, 0x30, 0X0D, 0X0A};//av = 10 enter
//const unsigned char speed_cmd[8] = {0x73, 0x70, 0x3D, 0x31, 0x30, 0x30, 0x0D, 0x0A}; //speed = 100 enter
//const unsigned char ros_data_cmd[6] = {0x73, 0x73, 0x3D, 0x37, 0x0D, 0x0A}; //ss = 7 enter

const char angle_cmd[] = "ang\r\n";
const char acc_cmd[] = "acc\r\n";
const char gyr_cmd[] = "gyr\r\n";
const char reset_angle_cmd[] = "zro\r\n";
const char reset_cmd[] = "zro\r\n";
const char av_cmd[] = "av=10\r\n";
const char speed_cmd[] = "sp=10\r\n";
const char ros_data_cmd[] = "ss=7\r\n";



const unsigned char A = 0x61;
const unsigned char N = 0x6E;
const unsigned char G = 0x67;
const unsigned char CR = 0x0D;
const unsigned char LF = 0x0A;

const double d2r = (M_PI /180.0);
const double r2d = 180.0 / M_PI;
const double g2a = 9.80665;
const double ut2t = 1000000;
const double tem_c = 1.0;


#endif // CONST_H

