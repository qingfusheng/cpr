#include "gat1400.h"
#include <stdio.h>
#include <string.h>
#include <syslog.h>
#include <time.h>
#include "gat_http_client.h"
#include "json.h"
#include "xlog.h"
#include "xhelper.h"
#include "curl/curl.h"

#define SERVERIP "120.55.20.214"
#define REGISTER "http://%s/VIID/System/Register"
#define UNREGISTER "http://%s/VIID/System/UnRegister"
#define KEEPALIVE "http://%s/VIID/System/Keepalive"
#define FACES "http://%s/VIID/Faces"
#define VEHICLES "http://%s/VIID/MotorVehicles"

#define IMGSVR "http://%s/VIID/ImageServer?DeviceID=%s"
#define IMGSEND "http://%.64s:%.3d/VIID/Images"
#define IMGREGISTER "http://%s:%d/VIID/System/Register"

// typedef struct _OAuth {
//   gchar name[32];
//   gchar password[32];
//   gchar realm[256];
//   gchar nonce[256];
//   gchar qop[256];
//   gchar opaque[256];
// } OAuth;

static char app_name[32];

static gchar *_deviceID = NULL;
//static OAuth *_header = NULL;

static gchar *_register_url = NULL;
static gchar *_unregister_url = NULL;
static gchar *_keepalive_url = NULL;
static gchar *_faces_url = NULL;
static gchar *_vehicles_url = NULL;

static gchar _username[64];
static gchar _password[64];

static gchar _upload_user[64];
static gchar _upload_pwd[64];

typedef struct _ImgSvrInfo {
  gchar upload_ip[128];
  int upload_port;
  gchar download_ip[128];
  int download_port;
  gchar pool[128];
  gchar requrl[128];
  gchar status[128];
  gchar code[128];
} ImageServerInfo;
typedef struct _BestNode {
  gchar ip[64];
  gchar token[64];
  gchar requrl[64];
  gchar status[64];
  int port;
  int code;
} BestNodeInfo;

ImageServerInfo _hk_img_svr;
BestNodeInfo _hk_best_node;

static gchar *_hk_img_server = NULL;
static gchar _hk_img_send_url[128];

void set_gat1400_appname(const char *application_name) {
  snprintf(app_name, 32, "%s", application_name);
}

// curl
static gboolean curl_service(const char *url, char *buffer);

//发送http请求
static gboolean curl_service(const char *url, char *buffer) {
  char header[1024];
  //设置头部信息
  snprintf(header, 1024, "Content-Type: application/json;charset=utf-8");
  gat_http_client_add_header(header);

  snprintf(header, 1024, "User-Identify: %s", _deviceID);
  gat_http_client_add_header(header);

  snprintf(header, 1024, "Connection: close");
  gat_http_client_add_header(header);

  snprintf(header, 1024, "Expect: ");
  gat_http_client_add_header(header);

  //GDateTime *tnow = g_date_time_new_now_utc();
  //gchar *timestr = g_date_time_format(time, "%Y%m%d%H%M%S");
  GDateTime *tutc = g_date_time_new_now_utc();
  //GDateTime *tnow = g_date_time_add_hours(tutc, 8);
  gchar *timestr = g_date_time_format(tutc, "%a, %d %b %Y %T GMT");
  snprintf(header, 1024, "Date: %s", timestr);
  gat_http_client_add_header(header);
  g_free(timestr);
  //g_date_time_unref(tnow);
  g_date_time_unref(tutc);

  gat_http_client_set_timeout(20L);

  logger_info("The info name is %s_info, detail info is curl_service url:%s\n", app_name, url);
  if (gat_http_client_run(GAT_HTTP_METHOD_POST, (char *)url, buffer) == 0) {
    logger_info("The info name is %s_info, detail info is curl_service success\n", app_name);
    return TRUE;
  } else {
    logger_warning("The exception name is %s_exception, detail info is curl_service error!\n", app_name);
    return FALSE;
  }
}

//发送http请求
static gboolean curl_service_get(const char *url, char * req_body, char (*header_list)[128], int header_cnt) {
  char header[1024];
  int i;

  //设置头部信息
  sprintf(header, "User-Identify:%s", _deviceID);
  gat_http_client_add_header(header);

  sprintf(header, "charset:utf-8");
  gat_http_client_add_header(header);

  sprintf(header, "Expect:");
  gat_http_client_add_header(header);

  if (header_list != NULL) {
    for (i = 0; i < header_cnt; ++i) {
      gat_http_client_add_header(header_list[i]);
    }
  }

  gat_http_client_set_timeout(20L);

  //logger_info("curl_service url:%s\n", url);
  if (gat_http_client_run(GAT_HTTP_METHOD_GET, (char *)url, req_body) == 0) {
    //logger_info("curl_service success\n");
    return TRUE;
  } else {
    //logger_warning("curl_service error!\n");
    return FALSE;
  }
}

gboolean setup_gat1400(char *deviceID, char *serverip, char *user, char *password, char *up_user, char *up_pwd) {
  snprintf(app_name, 32, "GatApp");

  _deviceID = g_strdup(deviceID);

  _register_url = g_strdup_printf(REGISTER, serverip);
  _unregister_url = g_strdup_printf(UNREGISTER, serverip);
  _keepalive_url = g_strdup_printf(KEEPALIVE, serverip);
  _faces_url = g_strdup_printf(FACES, serverip);
  _vehicles_url = g_strdup_printf(VEHICLES, serverip);
  _hk_img_server = g_strdup_printf(IMGSVR, serverip, _deviceID);

  snprintf(_username, 64, "%s", user);
  snprintf(_password, 64, "%s", password);

  snprintf(_upload_user, 64, "%s", up_user);
  snprintf(_upload_pwd, 64, "%s", up_pwd);

  logger_info("%s:%s  %s:%s", _username, _password, _upload_user, _upload_pwd);
  // setup_curl();
  return TRUE;
}

gboolean clean_gat1400() {
  g_free(_deviceID);

  g_free(_register_url);
  g_free(_unregister_url);
  g_free(_keepalive_url);
  g_free(_faces_url);
  g_free(_vehicles_url);
  g_free(_hk_img_server);

  return FALSE;
}
//注册
gboolean plc_register() {
  logger_info("The info name is %s_info, detail info is plc_register start", app_name);
  char *buffer;
  gboolean ret = FALSE;

  // json数据转换
  struct json_object *info_json = NULL;
  info_json = json_object_new_object();
  json_object_object_add(info_json, "DeviceID", json_object_new_string(_deviceID));
  json_object_object_add(info_json, "ProtocolVersion", json_object_new_string("2.0"));

  struct json_object *register_json = NULL;
  register_json = json_object_new_object();
  json_object_object_add(register_json, "RegisterObject", info_json);

  buffer = g_strdup(json_object_to_json_string(register_json));

  // register work
  if (gat_http_client_open() == 0) {
    long resp_code;
    gat_http_client_set_auth(_username, _password, GAT_HTTP_AUTH_DEFAULT);
    ret = curl_service(_register_url, buffer);
    resp_code = gat_http_client_get_resp_code();
    logger_info("The info name is %s_info, detail info is plc_register code: %ld", app_name, resp_code);
    char* url = gat_http_client_get_redirect_url();
    if(strlen(url) == 0) {
      logger_error("Register Error: fail to get redirect IP\n");
      ret = FALSE;
    } else {
      char buf[50];
      sscanf(url, "%*[^//]//%[^/]", buf);
      logger_info("redirect IP %s, length:%d", buf, strlen(buf));
      g_free(_register_url);
      g_free(_unregister_url);
      g_free(_keepalive_url);
      g_free(_faces_url);
      g_free(_vehicles_url);
      g_free(_hk_img_server);
      _register_url = g_strdup_printf(REGISTER, buf);
      _unregister_url = g_strdup_printf(UNREGISTER, buf);
      _keepalive_url = g_strdup_printf(KEEPALIVE, buf);
      _faces_url = g_strdup_printf(FACES, buf);
      _vehicles_url = g_strdup_printf(VEHICLES, buf);
      _hk_img_server = g_strdup_printf(IMGSVR, buf, _deviceID);
      gat_http_client_set_auth(_username, _password, GAT_HTTP_AUTH_DEFAULT);
      ret = curl_service(_register_url, buffer);
      resp_code = gat_http_client_get_resp_code();
      if (resp_code != 200) {
        logger_error("Register Error: fail to register redirect IP: %s\n", _register_url);
        ret = FALSE;
      }
    }

    gat_http_client_close();
  } else {
    ret = FALSE;
    logger_warning("The exception name is %s_exception, detail info is open http-client fail!", app_name);
  }

  g_free(buffer);
  json_object_put(register_json);

  return ret;
}

//注销
gboolean plc_unregister() {
  logger_info("The info name is %s_info, detail info is plc_unregister start\n", app_name);
  char *buffer;
  gboolean ret = FALSE;

  // json数据转换
  struct json_object *info_json = NULL;
  info_json = json_object_new_object();
  json_object_object_add(info_json, "DeviceID", json_object_new_string(_deviceID));

  struct json_object *unregister_json = NULL;
  unregister_json = json_object_new_object();
  json_object_object_add(unregister_json, "UnRegisterObject", info_json);

  buffer = g_strdup(json_object_to_json_string(unregister_json));

  if (gat_http_client_open() == 0) {
    long resp_code;
    ret = curl_service(_unregister_url, buffer);
    resp_code = gat_http_client_get_resp_code();
    logger_info("The info name is %s_info, detail info is plc_register code: %ld", app_name, resp_code);
    logger_info("The info name is %s_info, detail info is plc_unregister respond code: %d\n", app_name, (int)resp_code);
    if (resp_code != 200) {
      ret = FALSE;
    }
    gat_http_client_close();
  }

  g_free(buffer);
  json_object_put(unregister_json);

  return ret;
}

//心跳
gboolean plc_keepalive() {
  logger_info("The info name is %s_info, detail info is plc_keepalive start\n", app_name);
  char *buffer;
  gboolean ret = FALSE;

  // json数据转换
  struct json_object *info_json = NULL;
  info_json = json_object_new_object();
  json_object_object_add(info_json, "DeviceID", json_object_new_string(_deviceID));

  struct json_object *keepalive_json = NULL;
  keepalive_json = json_object_new_object();
  json_object_object_add(keepalive_json, "KeepaliveObject", info_json);

  buffer = g_strdup(json_object_to_json_string(keepalive_json));

  if (gat_http_client_open() == 0) {
    long resp_code;
    ret = curl_service(_keepalive_url, buffer);
    resp_code = gat_http_client_get_resp_code();
    logger_info("The info name is %s_info, detail info is plc_register code: %ld", app_name, resp_code);
    logger_info("The info name is %s_info, detail info is keep_alive respond code: %d\n", app_name, (int)resp_code);
    if (resp_code != 200) {
      ret = FALSE;
    }
    gat_http_client_close();
  }

  g_free(buffer);
  json_object_put(keepalive_json);

  return ret;
}

//上传人脸信息
gboolean plc_faceUpload(FaceInfo *face) {
  logger_info("The info name is %s_info, detail info is plc_face start\n", app_name);
  gboolean ret = FALSE;

  // json数据转换
  struct json_object *faceObjectList_json = NULL;
  faceObjectList_json = json_object_new_object();
  struct json_object *face_json = NULL;
  face_json = json_object_new_object();
  struct json_object *faceObject_json = NULL;
  faceObject_json = json_object_new_array();
  struct json_object *faceInfo_json = NULL;
  faceInfo_json = json_object_new_object();

  //子图片信息转json
  struct json_object *subImageList_json = NULL;
  subImageList_json = json_object_new_object();
  struct json_object *subImageInfoObject_json = NULL;
  subImageInfoObject_json = json_object_new_array();

  //背景图片信息
  //生成图片id
  struct json_object *imageInfo_json = NULL;
  imageInfo_json = json_object_new_object();
  char imageID[42] = "";
  char faceImageID[42] = "";
  char DeviceID[21] = "";

  snprintf(DeviceID, 21, "%s", _deviceID);
  strcat(imageID, DeviceID);
  strcat(imageID, "02");
  strcat(imageID, face->imageInfo.ShotTime);
  strcat(imageID, face->imageInfo.imageNum);

  logger_info("The info name is %s_info, detail info is mageID%s\n", app_name, imageID);

  json_object_object_add(imageInfo_json, "ImageID", json_object_new_string(imageID));
  json_object_object_add(imageInfo_json, "StoragePath", json_object_new_string(face->imageInfo.StoragePath));
  json_object_object_add(imageInfo_json, "Type", json_object_new_string("14"));  // 100-一般图片
  json_object_object_add(imageInfo_json, "FileFormat", json_object_new_string("Jpeg"));
  json_object_object_add(imageInfo_json, "ShotTime", json_object_new_string(face->imageInfo.ShotTime));
  json_object_object_add(imageInfo_json, "Width", json_object_new_int(face->imageInfo.Width));
  json_object_object_add(imageInfo_json, "Height", json_object_new_int(face->imageInfo.Height));

  // std::string imageData = "";
  gchar *imageData = NULL;
  if (face->imageInfo.imageData != NULL && face->imageInfo.imageSize != 0) imageData = g_base64_encode(face->imageInfo.imageData, face->imageInfo.imageSize);

  if (imageData != NULL) {
    json_object_object_add(imageInfo_json, "Data", json_object_new_string(imageData));
    g_free(imageData);
  } else
    json_object_object_add(imageInfo_json, "Data", json_object_new_string(""));

  json_object_array_add(subImageInfoObject_json, imageInfo_json);

  //人脸图片信息
  //生成人脸图片id
  struct json_object *faceImageInfo_json = NULL;
  faceImageInfo_json = json_object_new_object();
  snprintf(DeviceID, 21, "%s", _deviceID);
  strcat(faceImageID, DeviceID);
  strcat(faceImageID, "02");
  strcat(faceImageID, face->faceImageInfo.ShotTime);
  strcat(faceImageID, face->faceImageInfo.imageNum);

  logger_info("The info name is %s_info, detail info is faceImageID%s\n", app_name, faceImageID);

  json_object_object_add(faceImageInfo_json, "ImageID", json_object_new_string(faceImageID));
  json_object_object_add(faceImageInfo_json, "StoragePath", json_object_new_string(face->faceImageInfo.StoragePath));
  json_object_object_add(faceImageInfo_json, "Type", json_object_new_string("11"));  // 11-人脸图
  json_object_object_add(faceImageInfo_json, "FileFormat", json_object_new_string("Jpeg"));
  json_object_object_add(faceImageInfo_json, "ShotTime", json_object_new_string(face->faceImageInfo.ShotTime));
  json_object_object_add(faceImageInfo_json, "Width", json_object_new_int(face->faceImageInfo.Width));
  json_object_object_add(faceImageInfo_json, "Height", json_object_new_int(face->faceImageInfo.Height));
  // std::string faceImageData = "";
  /*
  gchar *faceImageData = NULL;
  if (face->faceImageInfo.imageData != NULL && face->faceImageInfo.imageSize != 0) faceImageData = g_base64_encode(face->faceImageInfo.imageData, face->faceImageInfo.imageSize);

  // g_message("%s", faceImageData);
  logger_info("The info name is %s_info, detail info is imageSize: %d", app_name, face->faceImageInfo.imageSize);
  if (faceImageData != NULL) {
    json_object_object_add(faceImageInfo_json, "Data", json_object_new_string(faceImageData));
    g_free(faceImageData);
  } else
    json_object_object_add(faceImageInfo_json, "Data", json_object_new_string(""));
  */
  json_object_array_add(subImageInfoObject_json, faceImageInfo_json);

  //人脸信息转json
  char faceID[49] = "";
  strcat(faceID, imageID);
  strcat(faceID, "06");
  strcat(faceID, face->faceNum);
  logger_info("The info name is %s_info, detail info is faceID: %s", app_name, faceID);
  json_object_object_add(faceInfo_json, "FaceID", json_object_new_string(faceID));
  json_object_object_add(faceInfo_json, "InfoKind", json_object_new_int(1));  // 1-自动采集
  json_object_object_add(faceInfo_json, "SourceID", json_object_new_string(imageID));
  json_object_object_add(faceInfo_json, "DeviceID", json_object_new_string(_deviceID));
  json_object_object_add(faceInfo_json, "LeftTopX", json_object_new_int(face->LeftTopX));
  json_object_object_add(faceInfo_json, "LeftTopY", json_object_new_int(face->LeftTopY));
  json_object_object_add(faceInfo_json, "RightBtmX", json_object_new_int(face->RightBtmX));
  json_object_object_add(faceInfo_json, "RightBtmY", json_object_new_int(face->RightBtmY));

  json_object_object_add(faceInfo_json, "LocationMarkTime", json_object_new_string(face->LocationMarkTime));
  json_object_object_add(faceInfo_json, "FaceAppearTime", json_object_new_string(face->FaceAppearTime));
  json_object_object_add(faceInfo_json, "FaceDisAppearTime", json_object_new_string(face->FaceDisAppearTime));
  json_object_object_add(faceInfo_json, "AgeUpLimit", json_object_new_int(80));
  json_object_object_add(faceInfo_json, "AgeLowerLimit", json_object_new_double(face->score));//int(10));
  json_object_object_add(faceInfo_json, "AccompanyNumber", json_object_new_int(face->AccompanyNumber));
  // json_object_object_add(faceInfo_json, "SkinColor", json_object_new_string(""));
  // json_object_object_add(faceInfo_json, "HairColor", json_object_new_string("1"));

  json_object_object_add(faceInfo_json, "IsDriver", json_object_new_int(face->id));//2));
  json_object_object_add(faceInfo_json, "IsForeigner", json_object_new_int(face->vsid));//2));
  json_object_object_add(faceInfo_json, "IsSuspectedTerrorist", json_object_new_int(2));
  json_object_object_add(faceInfo_json, "IsCriminalInvolved", json_object_new_int(2));
  json_object_object_add(faceInfo_json, "IsDetainees", json_object_new_int(2));
  json_object_object_add(faceInfo_json, "IsVictim", json_object_new_int(2));
  json_object_object_add(faceInfo_json, "IsSuspiciousPerson", json_object_new_int(2));
  json_object_object_add(faceInfo_json, "Attitude", json_object_new_int(1));
  json_object_object_add(faceInfo_json, "Similaritydegree", json_object_new_double(face->distance));//0));
  json_object_object_add(faceInfo_json, "EyebrowStyle", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "NoseStyle", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "MustacheStype", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "LipStype", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "WrinklePouch", json_object_new_string("4"));
  json_object_object_add(faceInfo_json, "AcneStain", json_object_new_string("4"));
  json_object_object_add(faceInfo_json, "FreckleBirthmark", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "ScarDimple", json_object_new_string("1"));
  json_object_object_add(faceInfo_json, "OtherFeature", json_object_new_string("8"));

  json_object_object_add(subImageList_json, "SubImageInfoObject", subImageInfoObject_json);
  json_object_object_add(faceInfo_json, "SubImageList", subImageList_json);
  json_object_array_add(faceObject_json, faceInfo_json);
  json_object_object_add(face_json, "FaceObject", faceObject_json);
  json_object_object_add(faceObjectList_json, "FaceListObject", face_json);

  char *buffer = g_strdup(json_object_to_json_string(faceObjectList_json));

  if (gat_http_client_open() == 0) {
    long resp_code;
    ret = curl_service(_faces_url, buffer);
    resp_code = gat_http_client_get_resp_code();
    logger_info("The info name is %s_info, detail info is plc_register code: %ld", app_name, resp_code);
    // g_message("face_upload respond code: %d\n", (int)resp_code);
    if (resp_code != 200) {
      ret = FALSE;
    }
    gat_http_client_close();
  }

  g_free(buffer);
  json_object_put(faceObjectList_json);

  return ret;
}

static void get_time_str(gchar* strtime, unsigned int strlen) {
  GDateTime* time = g_date_time_new_now_local();
  gchar *tmp_time = NULL;

  if (time) {
    tmp_time = g_date_time_format(time, "%Y%m%d%H%M%S");
    snprintf(strtime, strlen, "%s", tmp_time);    
    g_free(tmp_time);
    g_date_time_unref(time);
  } else {
    snprintf(strtime, strlen, "0");
  }
}

static void get_image_id(char *dest, unsigned int destlen, char *timestr, char *num) {
  snprintf(dest, destlen, "%s02%s%s", _deviceID, timestr, num);
}

static void getVehicleID(char id[49], const char srcID[42], const char objID[6]) {
  snprintf(id, 49, "%s02%s", srcID, objID);
}

gboolean plc_vehicleUpload(VehicleInfo *vehicle) {
  gboolean ret = FALSE;
  //struct json_object *jResult = json_object_new_object();
  struct json_object *jMotorVehicleListObject = json_object_new_object();
  struct json_object *jMotorVehicleElem = json_object_new_object();
  struct json_object *jMotorVehicleObject = json_object_new_array();
  char *reqbuf = NULL;
  struct json_object *jSubImageList = json_object_new_object();
  struct json_object *jSubImageInfoObj = json_object_new_array();
  char strtime[MAX_DATE_TIME_LEN];

  logger_info("The info name is %s_info, detail info is vehicle upload", app_name);
  get_time_str(strtime, MAX_DATE_TIME_LEN);
  char imgId[MAX_IMAGE_ID_LEN], vehicleID[49];
  if (vehicle->img_full.imageSize > 0) {
    get_image_id(imgId, MAX_IMAGE_ID_LEN, strtime, vehicle->img_full.imageNum);
    json_object_object_add(jMotorVehicleElem, "SourceID", json_object_new_string(imgId));
  }
  getVehicleID(vehicleID, imgId, "00001");
  json_object_object_add(jMotorVehicleElem, "MotorVehicleID", json_object_new_string(vehicleID));
  json_object_object_add(jMotorVehicleElem, "InfoKind", json_object_new_int(1));

  json_object_object_add(jMotorVehicleElem, "PlateNo", json_object_new_string(vehicle->PlateNo));
  json_object_object_add(jMotorVehicleElem, "PlateColor", json_object_new_string(vehicle->PlateColor));
  json_object_object_add(jMotorVehicleElem, "PlateReliability", json_object_new_string(vehicle->PlateReliability));
  json_object_object_add(jMotorVehicleElem, "LeftTopX", json_object_new_int(vehicle->LeftTopX));
  json_object_object_add(jMotorVehicleElem, "LeftTopY", json_object_new_int(vehicle->LeftTopY));
  json_object_object_add(jMotorVehicleElem, "RightBtmX", json_object_new_int(vehicle->RightBtmX));
  json_object_object_add(jMotorVehicleElem, "RightBtmY", json_object_new_int(vehicle->RightBtmY));

  json_object_object_add(jMotorVehicleElem, "DeviceID", json_object_new_string(_deviceID));
  json_object_object_add(jMotorVehicleElem, "StorageUrl1", json_object_new_string(vehicle->img_full.StoragePath));
  json_object_object_add(jMotorVehicleElem, "StorageUrl2", json_object_new_string(vehicle->img_plate.StoragePath));
  json_object_object_add(jMotorVehicleElem, "HasPlate", json_object_new_string("1"));
  json_object_object_add(jMotorVehicleElem, "Speed", json_object_new_int(0));
  json_object_object_add(jMotorVehicleElem, "PassTime", json_object_new_string(strtime));
  json_object_object_add(jMotorVehicleElem, "Sunvisor", json_object_new_int(0));
  json_object_object_add(jMotorVehicleElem, "SafetyBelt", json_object_new_int(1));
  json_object_object_add(jMotorVehicleElem, "Calling", json_object_new_int(0));
  json_object_object_add(jMotorVehicleElem, "LaneNo", json_object_new_int(0));
  json_object_object_add(jMotorVehicleElem, "BreakRuleMode", json_object_new_string(""));

  json_object_object_add(jMotorVehicleElem, "VehicleClass", json_object_new_string("K33"));
  json_object_object_add(jMotorVehicleElem, "VehicleColor", json_object_new_string("14"));
  json_object_object_add(jMotorVehicleElem, "VehicleColorDepth", json_object_new_string("1"));

  //scene sub image
  if (vehicle->img_full.imageSize > 0) {

    get_image_id(imgId, MAX_IMAGE_ID_LEN, strtime, vehicle->img_full.imageNum);
    struct json_object *jSubImg = json_object_new_object();
    //gchar *b64data = g_base64_encode(vehicle->img_full.imageData, vehicle->img_full.imageSize);
    json_object_object_add(jSubImg, "ImageID", json_object_new_string(imgId));
    json_object_object_add(jSubImg, "StoragePath", json_object_new_string(vehicle->img_full.StoragePath));
    json_object_object_add(jSubImg, "FileFormat", json_object_new_string("Jpeg"));
    json_object_object_add(jSubImg, "ShotTime", json_object_new_string(strtime));
    json_object_object_add(jSubImg, "Type", json_object_new_string("1"));
    json_object_object_add(jSubImg, "Width", json_object_new_int(vehicle->img_full.Width));
    json_object_object_add(jSubImg, "Height", json_object_new_int(vehicle->img_full.Height));
    //json_object_object_add(jSubImg, "Data", json_object_new_string(b64data));
    json_object_array_add(jSubImageInfoObj, jSubImg);
    //g_free(b64data);
  }
  
  //plate sub image
  if (vehicle->img_plate.imageSize > 0) {

    get_image_id(imgId, MAX_IMAGE_ID_LEN, strtime, vehicle->img_plate.imageNum);
    struct json_object *jSubImg = json_object_new_object();
    //gchar *b64data = g_base64_encode(vehicle->img_plate.imageData, vehicle->img_plate.imageSize);
    json_object_object_add(jSubImg, "ImageID", json_object_new_string(imgId));
    json_object_object_add(jSubImg, "StoragePath", json_object_new_string(vehicle->img_plate.StoragePath));
    json_object_object_add(jSubImg, "FileFormat", json_object_new_string("Jpeg"));
    json_object_object_add(jSubImg, "ShotTime", json_object_new_string(strtime));
    json_object_object_add(jSubImg, "Type", json_object_new_string("2"));
    json_object_object_add(jSubImg, "Width", json_object_new_int(vehicle->img_plate.Width));
    json_object_object_add(jSubImg, "Height", json_object_new_int(vehicle->img_plate.Height));
    //json_object_object_add(jSubImg, "Data", json_object_new_string(b64data));
    json_object_array_add(jSubImageInfoObj, jSubImg);
    //g_free(b64data);
  }

  json_object_object_add(jSubImageList, "SubImageInfoObject", jSubImageInfoObj);
  json_object_object_add(jMotorVehicleElem, "SubImageList", jSubImageList);
  json_object_array_add(jMotorVehicleObject, jMotorVehicleElem);
  json_object_object_add(jMotorVehicleListObject, "MotorVehicleObject", jMotorVehicleObject);
  //json_object_object_add(jResult, "MotorVehicleListObject", jMotorVehicleListObject);

  reqbuf = g_strdup(json_object_to_json_string(jMotorVehicleListObject));
  logger_info("uploading gat-vehicle object: %s", reqbuf);
  if (gat_http_client_open() == 0) {
    long resp_code;
    ret = curl_service(_vehicles_url, reqbuf);
    resp_code = gat_http_client_get_resp_code();
    logger_info("The info name is %s_info, detail info is resp code: %ld", app_name, resp_code);
    if (resp_code == 200) {
      ret = TRUE;
      logger_info("The info name is %s_info, detail info is uploading vehicle ok", app_name);
    } else {
      logger_warning("The warning name is %s_warn, detail info is invalid respond code:%ld", app_name, resp_code);
    }
    gat_http_client_close();
      } else {
    logger_error("The error name is %s_error, detail info is fail to open http client", app_name);
  }

  g_free(reqbuf);
  json_object_put(jMotorVehicleListObject);

  return ret;
}

static int load_hk_server_info(const char *str) {
  int ret = -1;
  json_object *jresp = json_tokener_parse(str);
  json_object *juser = NULL;
  json_object *jdownload_ip = NULL;
  json_object *jdownload_port = NULL;
  json_object *jpool = NULL;
  json_object *jrequrl = NULL;
  json_object *jcode = NULL;
  json_object *jstatus = NULL;
  json_object *jupload_ip = NULL;
  json_object *jupload_port = NULL;


  juser = json_object_object_get(jresp, "ResponseStatusObject");
  if (!juser) {
    goto Exit;
  }

  jrequrl = json_object_object_get(juser, "RequestURL");
  if (!jrequrl) {
    goto Exit;
  }
  snprintf(_hk_img_svr.requrl, 128, "%s", json_object_get_string(jrequrl));

  jcode = json_object_object_get(juser, "StatusCode");
  if (!jcode) {
    goto Exit;
  }
  snprintf(_hk_img_svr.code, 128, "%s", json_object_get_string(jcode));

  jstatus = json_object_object_get(juser, "StatusString");
  if (!jstatus) {
    goto Exit;
  }
  snprintf(_hk_img_svr.status, 128, "%s", json_object_get_string(jstatus)); 

  jupload_ip = json_object_object_get(juser, "UploadIP");
  if (!jupload_ip) {
    goto Exit;
  }
  snprintf(_hk_img_svr.upload_ip, 128, "%s", json_object_get_string(jupload_ip));

  jupload_port = json_object_object_get(juser, "UploadPort");
  if (!jupload_port) {
    goto Exit;
  }
  _hk_img_svr.upload_port = json_object_get_int(jupload_port); 

  jdownload_ip = json_object_object_get(juser, "DownloadIP");
  if (!jdownload_ip) {
    goto Exit;
  }
  snprintf(_hk_img_svr.download_ip, 128, "%s", json_object_get_string(jdownload_ip));

  jdownload_port = json_object_object_get(juser, "DownloadPort");
  if (!jdownload_port) {
    goto Exit;
  }
  _hk_img_svr.download_port = json_object_get_int(jdownload_port); 

  jpool = json_object_object_get(juser, "PoolID");
  if (!jpool) {
    goto Exit;
  }
  snprintf(_hk_img_svr.pool, 128, "%s", json_object_get_string(jpool));

  logger_info("parse image server OK");

  ret = 0;

Exit:
  if (jresp) {
    json_object_put(jresp);
  }
  
  return ret;
}

static gboolean hk_get_img_server(const char *url) {
  gboolean ret = TRUE;

  if (gat_http_client_open() == 0) {
    long resp_code;
    char *resp_body;
    unsigned int resp_size;

    gat_http_client_set_auth(_upload_user, _upload_pwd, GAT_HTTP_AUTH_DEFAULT);
    ret = curl_service_get(url, NULL, NULL, 0);
    resp_code = gat_http_client_get_resp_code();
    logger_info("respond code %ld", resp_code);
    if (resp_code != 200) {
      logger_error("image server:invalid respond code %ld", resp_code);
      ret = FALSE;
      goto finish;
    }else{
      char tmp[GAT_RESP_SIZE];
      resp_body = gat_http_client_get_resp_body(&resp_size);
      snprintf(tmp, GAT_RESP_SIZE, "%s", resp_body);
      logger_info("image server resp:%s", tmp);
      if (load_hk_server_info(tmp)) {
        logger_error("fail to parse image server respond");
        ret = FALSE;
        goto finish;
      }

      logger_info(">>>code:%s,status:%s", _hk_img_svr.code, _hk_img_svr.status);
      logger_info(">>>up-ip:%s,up-port:%d,url:%s", _hk_img_svr.upload_ip, _hk_img_svr.upload_port, _hk_img_svr.requrl);
      logger_info(">>>down-ip:%s,down-port:%d,PoolID:%s", _hk_img_svr.download_ip, _hk_img_svr.download_port, _hk_img_svr.pool);
    }

  }else {
    return FALSE;
  }

finish:
  gat_http_client_close();
  return ret;
}

// static void hk_get_auth_code(const char *source_data, const char *digest_key, char *digest, int digest_size) {
//   int source_data_len = strlen(source_data);
//   int digest_key_len = strlen(digest_key);
//   char output[256];
//   int output_len = 0;

//   GetDataDigest(source_data, source_data_len, digest_key, digest_key_len, output, &output_len);

//   snprintf(digest, digest_size, "%s", output);

//   return;
// }

// static void get_local_time(char *time_str, int time_len) {
//   time_t rawtime;
//   struct tm * timeinfo;
 
//   time(&rawtime);

//   timeinfo = localtime(&rawtime);

//   strftime(time_str, time_len, "%a, %d %b %Y %H:%M:%S GMT", timeinfo);

//   return;
// }

gboolean hk_img_register(const char *url) {
  gboolean ret;

  if (gat_http_client_open() == 0) {
    long resp_code;
    char reqbody[512];
    char *respbody;
    unsigned int respbody_size;

    json_object *jReq = json_object_new_object();
    json_object *jRegObj = json_object_new_object();
    json_object_object_add(jRegObj, "DeviceID", json_object_new_string(_deviceID));
    json_object_object_add(jRegObj, "ProtocolVersion", json_object_new_string("2.0"));
    json_object_object_add(jReq, "RegisterObject", jRegObj);
    snprintf(reqbody, 512, "%s", json_object_to_json_string(jReq));

    logger_info("img register url:%s, body:%s", url, reqbody);
    json_object_put(jReq);

    GChecksum *gcsum = g_checksum_new(G_CHECKSUM_MD5);
    g_checksum_update(gcsum, (guchar*)_upload_pwd, -1);
    gchar * md5 = (gchar*)g_checksum_get_string(gcsum);
    gat_http_client_set_auth(_upload_user, md5, GAT_HTTP_AUTH_DEFAULT);
    logger_info("username: %s, password: %s, md5: %s", _upload_user, _upload_pwd, md5);
    ret = curl_service(url, reqbody);
    if (ret == FALSE) {
      logger_error("curl error: img register");
      return FALSE;
    }
    resp_code = gat_http_client_get_resp_code();
    if (resp_code != 200) {
      logger_error("invalid img register respond code:%ld", resp_code);
      ret = FALSE;
    }

    respbody = gat_http_client_get_resp_body(&respbody_size);
    logger_info("img register succeed, respond:%s", respbody);

    gat_http_client_close();
  }

  return TRUE;
}

gboolean hk_connect() {
  char img_register_url[255];

  logger_info("start hk connect");

  if (hk_get_img_server(_hk_img_server) == FALSE) {
    logger_error("fail to get image server");
    return FALSE;
  }

  if (plc_keepalive() == FALSE) {
    logger_error("fail to do system_keepalive");
    return FALSE;
  }else{
    logger_info("system_keepalive finish!");
  }

  if (img_register() == FALSE) {
    logger_error("fail to register image server %s", img_register_url);
    return FALSE;
  }
  
  snprintf(_hk_img_send_url, 128, IMGSEND, _hk_img_svr.upload_ip, _hk_img_svr.upload_port);
  
  logger_info(">>>send image url:%s", _hk_img_send_url);

  return TRUE;
}

gboolean img_register() {
  char img_register_url[255];

  snprintf(img_register_url, 255, IMGREGISTER, _hk_img_svr.upload_ip, _hk_img_svr.upload_port);
  if (hk_img_register(img_register_url) == FALSE) {
    logger_error("fail to register image server:%s", img_register_url);
    return FALSE;
  }

  return TRUE;
}

char g_img_upload_resp_buf[GAT_RESP_SIZE];

static size_t hk_cb_data_writer(void* data, size_t size, size_t nmemb, void* userdata)
{
    long totalSize = size * nmemb;
    (void)userdata;

    memset(g_img_upload_resp_buf, 0x0, GAT_RESP_SIZE);
    memcpy(g_img_upload_resp_buf, (char*)data, (totalSize > (GAT_RESP_SIZE - 1) ? (GAT_RESP_SIZE - 1) : totalSize));

    //printf("resp body>>>%.*s\n", (int)totalSize, (char*)data);
    logger_info("image upload respond:%s", g_img_upload_resp_buf);

    return totalSize;
}

static int curl_img_upload(const char *infodata, const char *bufdata, unsigned int bufsize, char *img_url, unsigned int img_url_len) {
  CURL *curl;
  long respcode = 0;
  const char *url = _hk_img_send_url;

  curl_global_init(CURL_GLOBAL_ALL);
  curl = curl_easy_init();
  if (!curl) {
    printf("libcurl:fail to init\n");
    return -1;
  }else{
    CURLcode res = CURLE_OK;
    curl_mime *mime = NULL;
    curl_mimepart *part = NULL;

    mime = curl_mime_init(curl);

    part = curl_mime_addpart(mime);
    curl_mime_data(part, infodata, CURL_ZERO_TERMINATED);
    curl_mime_type(part, "application/VIID+JSON; charset=UTF-8");
    curl_mime_name(part, "ImageInfo");
    // struct curl_slist *headers = NULL;
    // headers = curl_slist_append(headers, "Custom-Header: mooo"); 
    // curl_mime_headers(part, headers, TRUE);

    part = curl_mime_addpart(mime);
    curl_mime_filedata(part, "/usr/local/packages/FaceR/param.conf");
    curl_mime_data(part, bufdata, bufsize);
    curl_mime_name(part, "Image");
    curl_mime_type(part, "image/jpeg");

    curl_easy_setopt(curl, CURLOPT_MIMEPOST, mime);

    struct curl_slist *post_headers = NULL;
    char str_header[128];
    snprintf(str_header, 128, "User-Identify: %s", _deviceID);
    post_headers = curl_slist_append(post_headers, str_header);
    post_headers = curl_slist_append(post_headers, "Connection: keep-alive");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, post_headers);

    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, hk_cb_data_writer);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, NULL);

    curl_easy_setopt(curl, CURLOPT_URL, url);

    /* Send the message */
    logger_info("start to post url:%s", url);
    res = curl_easy_perform(curl);

    if (res != CURLE_OK) {
      xapp_fatal_exit("perform error");
    }

    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &respcode);

    curl_slist_free_all(post_headers);
    //curl_slist_free_all(headers);
    curl_easy_cleanup(curl);
    curl_mime_free(mime);
    curl_global_cleanup();

    logger_info("perform OK!");
  }

  if (respcode != 200) {
    logger_error("image_upload fail to post:%s, respond code: %ld != 200", url, respcode);
    logger_error("respond body:%s", g_img_upload_resp_buf);
    return -1;
  }else{
    logger_info("image_upload succeed to post:%s, respond body:%s", url, g_img_upload_resp_buf);
    json_object *jresp = json_tokener_parse(g_img_upload_resp_buf);
    json_object *jresult = NULL;
    json_object *jurl = NULL;
    json_object *jstatus = NULL;

    jresult = json_object_object_get(jresp, "ResponseStatusObject");
    if (!jresult) {
      json_object_put(jresp);
      logger_error("fail to parse result field: ResponseStatusObject");
      return -1;
    }

    jurl = json_object_object_get(jresult, "Url");
    if (!jurl) {
      json_object_put(jresp);
      logger_error("fail to parse result field: Url");
      return -1;
    }
    snprintf(img_url, img_url_len, "%s", json_object_get_string(jurl));
    logger_info("image_upload url:%s", img_url);

    jstatus = json_object_object_get(jresult, "StatusString");
    if (!jstatus) {
      json_object_put(jresp);
      logger_error("can not find field: StatusString");
      return -1;
    }
    logger_info("upload result string:%s", json_object_get_string(jstatus));
    json_object_put(jresp);
  }

  return 0;
}

int hk_img_upload(const char *bufdata, unsigned int bufsize, char *download_url, unsigned int url_len) {
  int ret;
  char tmp[64];
  json_object *jreq = json_object_new_object();
  json_object *jImgList = json_object_new_object();
  json_object *jImgObj = json_object_new_array();
  json_object *jImgObjElem = json_object_new_object();
  json_object *jImgInfo = json_object_new_object();

  json_object_object_add(jreq, "ImageListObject", jImgList);
  json_object_object_add(jImgList, "ImageObject", jImgObj);
  json_object_array_add(jImgObj, jImgObjElem);
  json_object_object_add(jImgObjElem, "ImageInfo", jImgInfo);

  json_object_object_add(jImgInfo, "ImageID", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "InfoKind", json_object_new_int(0));
  json_object_object_add(jImgInfo, "ImageSource", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "SourceVideoID", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "OriginImageID", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "EventSort", json_object_new_int(0));
  json_object_object_add(jImgInfo, "DeviceID", json_object_new_string(_deviceID));
  json_object_object_add(jImgInfo, "StoragePath", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "FileHash", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "FileFormat", json_object_new_string("Jpeg"));
  snprintf(tmp, 64, "%u000", (unsigned int)time(NULL));
  json_object_object_add(jImgInfo, "ShotTime", json_object_new_string(tmp));
  json_object_object_add(jImgInfo, "Title", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "TitleNote", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "SpecialName", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "Keyword", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "ContentDescription", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "SubjectCharacter", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "SecurityLevel", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "Width", json_object_new_int(0));
  json_object_object_add(jImgInfo, "Height", json_object_new_int(0));
  json_object_object_add(jImgInfo, "ImageProcFlag", json_object_new_int(0));
  json_object_object_add(jImgInfo, "ShotPlaceFullAdress", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "ShotPlaceLongitude", json_object_new_int(0));
  json_object_object_add(jImgInfo, "ShotPlaceLatitude", json_object_new_int(0));
  json_object_object_add(jImgInfo, "HorizontalShotDirection", json_object_new_string("0"));
  json_object_object_add(jImgInfo, "VerticalShotDirection", json_object_new_string("0"));

  snprintf(tmp, 64, "%u", bufsize);
  json_object_object_add(jImgInfo, "FileSize", json_object_new_string(tmp));
  json_object_object_add(jImgInfo, "PoolID", json_object_new_string(_hk_img_svr.pool));
  json_object_object_add(jImgInfo, "PicType", json_object_new_string("01"));  

  gchar *strinfo = g_strdup(json_object_to_json_string(jreq));
  char url_resp[256];
  memset(url_resp, 0x0, sizeof(url_resp));
  ret = curl_img_upload(strinfo, bufdata, bufsize, url_resp, 256);

  snprintf(download_url, url_len, "http://%s:%d%s", _hk_img_svr.upload_ip, 6501, url_resp);
  logger_info("final image storage path %s", download_url);

  g_free(strinfo);
  json_object_put(jreq);
  return ret;
}
