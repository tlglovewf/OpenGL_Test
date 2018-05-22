//
//  CCommonHelper.h
//  
//  通用计算方法类(静态类)
//
//  Created by TuLigen on 16/3/16.
//
//

#ifndef ____CCommonHelper__
#define ____CCommonHelper__
#include "MpBase.h"
#include "Com_TypeDef.h"
#include "SDrawVertex.h"
#include "irr/quaternion.h"
#include "IEarthModel.h"
#include <string>
namespace mp
{
    namespace engine
    {
        class IMpViewCamera;
    }
    namespace draw
    {
        class IDrawState;
    }
    
    
    namespace help
    {
        
        
        /// 通用方法帮助类
        class MPSDK_API GlobeFunction
        {
        public:
            /**
             *  向量旋转90度
             *
             *  @param p1 输入向量
             *
             *  @return 旋转后的向量
             */
            static inline Vector2df RotateBy90(const Vector2df& p1)
            {
                return Vector2df(-p1.Y,p1.X);
            }
            
            /**
             *  Vector2df转Vector3df，默认Z为0
             *
             *  @param point 输入Vector2df
             *
             *  @return 输出Vector3df
             */
            static inline Vector3df ToVector3d(Vector2df point)
            {
                return Vector3df(point.X,point.Y,0);
            }
            
            /**
             *  Vector2df转Vector3df
             *
             *  @param point 输入Vector2df
             *  @param z     Z值
             *
             *  @return 输出Vector3df
             */
            static inline Vector3df ToVector3d(Vector2df point,double z)
            {
                return Vector3df(point.X,point.Y,z);
            }
            
            /**
             *  Vector3df转Vector2df，取Vector3df X，Y
             *
             *  @param point 输入Vector3df
             *
             *  @return 输出Vector2df
             */
            static inline Vector2df ToVector2d(Vector3df point)
            {
                return Vector2df(point.X,point.Y);
            }

            /**
             *  获取面对相机四元数
             *
             *  @param pCam 相机
             *
             *  @return 四元数
             */
            static  irr::core::quaternion FaceToCameraQua(mp::engine::IMpViewCamera *pCam);
            
            /**
             *  根据目录创建纹理偏移图
             *
             *  @param respath 资源目录路径
             *  @param index   索引值
             *
             *  @return 是否创建成功
             */
            static bool UpdateTextureAtlasFromFolder(const char *respath,int index);
            
            /**
             *  纹理偏移图中插入图片
             *
             *  @param filepath 图片路径
             *  @param index    偏移图索引
             *
             *  @return 是否插入成功
             */
            static bool InsertTextureToTextureAtlas(const char *filepath,const char *filename,int index);
            
            /**
             *  将内存地址转换成惟一id
             *
             *  @param pcString   内存地址
             *  @param ulHashType hash类型
             *  @param a_iLen     长度
             *
             *  @return id
             */
            static u32 HashStringAlgo( u8* pcString, u32 ulHashType,u32 a_iLen );
         
            /**
             *  计算屏幕世界矩阵
             *
             *  @param pDrawState drawstate指针
             *  @param bound          屏幕范围
             */
            static void BuildOthoDrawState(mp::draw::IDrawState *pDrawState, const Rect2di &bound);
            
            /**
             *  获取屏幕世界矩阵
             *
             *  @param matrix 屏幕世界矩阵
             *  @param bound  屏幕范围
             */
            static void GetMatrixFromScreen( mp::Matrix4f &matrix, const Rect2di &bound);
            
            /**
             *  根据路径获取显示要素
             *
             *  @param type 显示要素类型
             *
             *  @return 半径大小
             */
            //static float GetRenderRaidusByPath(int type) ;
            
            /**
             *  utf8->unicode
             *
             *  @param UTF8String              utf8
             *  @param UTF8StringLength        长度
             *  @param OutUnicodeString        unicode
             *  @param UnicodeStringBufferSize 长度
             *
             *  @return 长度
             */
            static u32 UTF8StrToUnicode( const char* UTF8String, u32 UTF8StringLength, WCHAR* OutUnicodeString, u32 UnicodeStringBufferSize );
            
            /**
             *  三角剖分
             *
             *  @param inarray  输入顶点坐标
             *  @param outarray 输出顶点坐标集合
             *  @param color    颜色
             *  @param idx      索引
             */
            static void GPCSimpleTriangulate( const PosArray &inarray, mp::PosClrArray *outarray,u32 color = 0xff000000,  mp::IndexArray *idx = NULL);
            static void GPCSimpleTriangulate( const Vector3df *inarray, int posSize, mp::PosClrArray *outarray,u32 color = 0xff000000,  mp::IndexArray *idx = NULL,float zindex = 0.0);
            
            static void GPCComplexTraiangulate( const std::vector<PosArray> &inarray,mp::PosClrArray *outarray, mp::IndexArray *idx ,u32 color = 0xff000000);
            
            /**
             *  折线加宽
             *
             *  @param inarray   输入顶点坐标
             *  @param outarray  输出顶点坐标集合
             *  @param color     颜色
             *  @param idx       索引
             *  @param lineWidth 线宽
             */
            static void BoldLine(const PosArray &inarray, mp::PosClrArray *outarray,u32 color, mp::IndexArray *idx,float lineWidth,float zidx = 0);
            static void BoldLine( const Vector3df *pPoints, int nPoints, mp::PosClrArray *outarray,u32 color = 0xff000000,  mp::IndexArray *idx = NULL, float lineWidth = 10, float zidx = 0);
            /**
             *  构建点精灵网格
             *
             *  @param pos     点坐标
             *  @param array   顶点数组
             *  @param indices 索引数组
             *  @param radius  半径
             *  @param offset  偏移量
             */
            static void BuildPointSpriteMesh(const Vector3df &pos, PosUvArray &array, IndexArray &indices, float radius = 10,Vector3df offset = Vector3df(0,0,0));
            
            /**
             *  根据经纬度数组计算世界坐标包围盒
             *
             *  @param positionArray 数组指针
             *  @param num           数组长度
             */
            static Box2dd GetBoundBox(const GPosition* positionArray,int num);
            
            /*  计算旋转
             *
             *  @param pos  坐标
             *  @param cosx cos值
             *  @param sinx sin值
             *  @param rate 缩放比
             *
             *  @return 返回值
             */
            static inline Vector3df RotateTrans(const Vector3df &pos,float cosx, float sinx, float rate)
            {
                float x = (pos.X * cosx - pos.Y * sinx) * rate;
                float y = (pos.X * sinx + pos.Y * cosx) * rate;
                return Vector3df(x,y,0);
            }
            
            static inline Vector3df RotateTrans(const Vector3df &pos,float cosx, float sinx)
            {
                float x = pos.X * cosx - pos.Y * sinx;
                float y = pos.X * sinx + pos.Y * cosx;
                return Vector3df(x,y,0);
            }
            
            /**
             *  计算包围盒长宽(地理坐标包围盒)
             *
             *  @param box 包围盒
             *
             */
            static void CalcBoxSize( const Box2dd &box);
            
            /**
             *  计算包围盒长宽(地理坐标包围盒)
             *
             *  @param pt1
             *  @param pt2
             *
             *  return 距离
             *
             */
            static double ComputeDistanceFromGeo(const GPosition &gp1, const GPosition &gp2)
            {
                double a = gp1.X - gp2.X;
                double b = gp1.Y - gp2.Y;
                double dis = 2 * asin(sqrt(pow(sin(a/2),2)+cos(gp1.X)*cos(gp2.X)*pow(sin(b/2),2)));
                dis = dis * Earth_Equatorial_Radius;//地球半径
                return dis;
            }
            
        };
//预定义宏
#define DECALREGLOBEVAR(TYPE, VAR) static TYPE g_##VAR;
#define INITGLOBEVAR(VAR,VALUE,TYPE)TYPE GlobalVariables::g_##VAR = VALUE;
#define GLOBAEVARNAME(T)  g_##T
#define DECALRESETTINGFUNC(TYPE,VAR) \
       static inline void Set##VAR(TYPE var){GLOBAEVARNAME(VAR) = var;}\
       static inline TYPE Get##VAR(){return GLOBAEVARNAME(VAR);}
#define DECALREFUNC(TYPE,VAR)\
static void Set##VAR(TYPE var);\
static TYPE Get##VAR();
        
        /// 全局变量类
        class MPSDK_API GlobalVariables
        {
        public:
            /**
             *  语言
             *
             */
            DECALRESETTINGFUNC(u16,LanguageCode)
            
            /**
             *  dpi
             *
             */
            DECALRESETTINGFUNC(float, DpiRate)
        
            /**
             *  屏幕矩阵
             *
             */
            DECALRESETTINGFUNC(mp::Matrix4f, ScreenMatrix)
         
            
            /**
             *  设置比例尺显示位置
             *
             *  @param mp::Vector2di  屏幕坐标
             */
            DECALRESETTINGFUNC(mp::Vector2di, ScaleCursorPos)
            
            /**
             *  游标可见性
             *
             *  @param bool 可见性
             *
             */
            DECALRESETTINGFUNC(bool, ScaleCursorVisible)
            
            /**
             *  调试信息可见性
             *
             *  @param bool 可见性
             */
            DECALRESETTINGFUNC(bool, DebugInfoVisible);
            
            /**
             *  设置休眠时间
             *  @param 毫秒
             */
            DECALRESETTINGFUNC(u32, SleepTime);
            
            /**
             *  辅助线程休眠
             *  @param 是否休眠
             */
            DECALRESETTINGFUNC(bool, ThreadSleep);
            
            /**
             *  辅助线程休眠时间
             *  @param 休眠时间间隔
             *
             */
            DECALRESETTINGFUNC(bool, ThreadSleepInterval);
        private:
          
            DECALREGLOBEVAR(u16          , LanguageCode)             //语言
            DECALREGLOBEVAR(float        , DpiRate)                  //dpirate
            DECALREGLOBEVAR(mp::Matrix4f , ScreenMatrix)             //屏幕矩阵
            DECALREGLOBEVAR(mp::Vector2di, ScaleCursorPos)           //游标位置
            DECALREGLOBEVAR(bool         , ScaleCursorVisible)       //游标可见性
            DECALREGLOBEVAR(bool         , DebugInfoVisible)         //debug信息可见性
            DECALREGLOBEVAR(u32          , SleepTime)                //休眠时间(毫秒)
            DECALREGLOBEVAR(bool         , ThreadSleep)              //辅助线程休眠
            DECALREGLOBEVAR(int          , ThreadSleepInterval)      //辅助线程休眠时间(毫秒)
            GlobalVariables();
        };
    }
}


#endif /* defined(____CCommonHelper__) */
