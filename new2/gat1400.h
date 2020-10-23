#ifndef _GAT1400_H_
#define _GAT1400_H_
#include <glib.h>

#define GAT_RESP_SIZE (1024*16) //16KB

#define MAX_PLATE_LEN 15
#define MAX_DATE_TIME_LEN 15
#define MAX_DEVICE_LEN 128
#define MAX_URL_LEN 255
#define MAX_COLOR_TYPE_LEN 3
#define MAX_IMAGE_ID_LEN 42

typedef struct _SubImageInfo {
  // char ImageID[50];//图片标识 41位
  char StoragePath[257];  //存储路径 256位
  char type[4];           //图像类型 默认11-人脸图 3位
  // char FileFormat[10];//图片格式 默认Jpeg 6位
  char ShotTime[15];  //拍摄时间YYYYMMDDhhmmss 14位
  int Width;
  int Height;
  char filePath[200];        //文件路径
  unsigned char *imageData;  //图片数据
  int imageSize;             //图像大小字节数
  char imageNum[6];          //用于图片id编码，同一时间生成的图片，编码需要递增，5位，00001
} SubImageInfo;

typedef struct _FaceInfo {
  // char FaceID[50];//人脸标识 48位
  // int InfoKind;//信息分类 1-自动采集 默认2-人工采集
  // char SourceID[50];//来源图像信息标识 41位
  char DeviceID[21];  //设备编码，自动采集必选 20位
  int LeftTopX;       //左上角 X 坐标
  int LeftTopY;       //左上角 Y 坐标
  int RightBtmX;      //右下角 X 坐标
  int RightBtmY;      //右下角 Y 坐标

  int AccompanyNumber;  // face count detected at the same time

  char LocationMarkTime[15];   //位置标记时间YYYYMMDDhhmmss 14位
  char FaceAppearTime[15];     //人脸出现时间YYYYMMDDhhmmss 14位
  char FaceDisAppearTime[15];  //人脸消失时间YYYYMMDDhhmmss 14位

  // int IsDriver;//是否驾驶员 0：否；1：是；2：不确定
  // int IsForeigner;//是否涉外人员 0：否；1：是；2：不确定
  // int IsSuspectedTerrorist;//是否涉恐人员 0：否；1：是；2：不确定
  // int IsCriminalInvolved;//是否涉案人员 0：否；1：是；2：不确定
  // int IsDetainees;//是否在押人员 0：否；1：是；2：不确定，人工采集必填
  // int IsVictim;//是否被害人 0：否；1：是；2：不确定
  // int IsSuspiciousPerson;//是否可疑人 0：否；1：是；2：不确定

  char faceNum[6];             //用于人脸id编码，同一张图片识别出来的人脸，编码需要递增，5位，00001
  SubImageInfo imageInfo;      //背景图片
  SubImageInfo faceImageInfo;  //人脸图片

  long id, vsid;
  float distance, score;
} FaceInfo;

// typedef struct _subimageinfo {
//   int ImageID;
//   char ShotTime[MAX_DATE_TIME];
//   int Width;
//   int Height;
//   char *Data;
// } SubImageInfo;

typedef struct _vehicleinfo {
  int LeftTopX;
  int LeftTopY;
  int RightBtmX;
  int RightBtmY;
  char PlateNo[MAX_PLATE_LEN];
  char PlateColor[MAX_COLOR_TYPE_LEN];
  char PlateReliability[3];
  char PlateCharReliability[64];
  SubImageInfo img_full;
  SubImageInfo img_plate;
} VehicleInfo;

void set_gat1400_appname(const char *app_name);

gboolean setup_gat1400(char *deviceID, char *serverip, char *user, char *password, char *up_user, char *up_pwd);
gboolean clean_gat1400();
//注册
gboolean plc_register();
//注销
gboolean plc_unregister();
//心跳
gboolean plc_keepalive();
//上传人脸信息
gboolean plc_faceUpload(FaceInfo *face);
// upload vehicle info
gboolean plc_vehicleUpload(VehicleInfo *vehicle);

gboolean hk_connect();
gboolean img_register();
//int testjson(const char *str);
int hk_img_upload(const char *bufdata, unsigned int bufsize, char *download_url, unsigned int url_len);

#endif
