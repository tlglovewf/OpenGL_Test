//
//  CCommonHelper
//  
//  主要用于一些通用计算
//
//  Created by TuLigen on 16/3/16.
//
//

#include "CCommonHelper.h"
#include "IMpViewCamera.h"
#include "Com_File.h"
#include "Com_Log.h"
#include "Com_Memory.h"
#include "CTexAtlasManager.h"
#include "irr/IImage.h"
#include "CMpImageFactory.h"
#include "IDrawContext.h"
#include <sstream>
#ifdef ANDROID
extern "C"{
#include "gpc.h"
}
#else
#include "gpc.h"
#endif
#include <assert.h>
namespace mp {
    namespace help
    {
        /**
         *  获取面对相机四元数
         *
         *  @param pCam 相机
         *
         *  @return 四元数
         */
        irr::core::quaternion GlobeFunction::FaceToCameraQua(mp::engine::IMpViewCamera *pCam)
        {
            if( NULL != pCam )
            {
                irr::core::quaternion pitchqua;
                irr::core::quaternion yawqua;
                irr::core::quaternion qua;
                pitchqua.fromAngleAxis(-pCam->getPitch(), Vector3df(1,0,0));
                yawqua.fromAngleAxis(pCam->getYaw(), Vector3df(0,0,1));
                return  yawqua * pitchqua;
            }
            else
            {
                return irr::core::quaternion();
            }
        }
        
        /**
         *  比较名称(主要用于map容器,名称比较)
         *
         *  @param lft 左值
         *  @param rgt 右值
         *
         *  @return 是否相同
         */
        static bool CmpPicName(const char *lft,const char *rgt)
        {
            if(NULL == lft || NULL == rgt )
                return false;
            char tmp[255] = {0};
            sprintf( tmp, "%s.png", rgt );
            int rst1 = strcmp( tmp, lft );
            int rst2 = strcmp(lft, rgt);
            return (0 == rst1 || 0 == rst2) ? false : rst2 > 0;
        }
        
        
        /**
         *  根据目录创建纹理偏移图
         *
         *  @param respath 资源目录路径
         *  @param index   索引值
         *
         *  @return 是否创建成功
         */
        bool GlobeFunction::UpdateTextureAtlasFromFolder(const char *respath,int index)
        {
            if( NULL == respath || index < 0 )
            {
                return false;
            }
            ST_FILENAMELIST *filelist = NULL;
            ULONG numFiles = 0;
            char path[255] = {0};
          
            Com_GetFilesUnderFolder(respath, true, "png", &filelist, &numFiles);
            irr::video::IImage *bigimg =  CTexAtlasManager::getSingleton().getAtlasImage(index);
            if(NULL == bigimg)
            {
                CTexAtlasManager::getSingleton().createAtlasMap(index,CmpPicName);
            }
           
            for(int idx = 0 ; idx < numFiles; ++idx )
            {
                size_t szLen = strlen(filelist[idx].cFileName);
                if( 0 < szLen)
                {//防止出现空名称的图片,所有图片必须命名
                    sprintf(path, "%s/%s",filelist[idx].cFilePath,filelist[idx].cFileName);
                    video::IImage *pimg =  mp::base::IMpSite::getSite()->getImageFactory()->load(path);
                    if(NULL == pimg )
                        continue;
                    if(pimg->getColorFormat() == irr::video::ECF_R8G8B8)
                    {
                        printf("Format not A8R8G8B8 : %s - %d\n",filelist[idx].cFileName, pimg->getColorFormat());
                    }
                    
                    TexUnitInfo *info = new TexUnitInfo;
                    info->img = pimg;
                    info->name = new char[szLen + 1];
                    strcpy( info->name, filelist[idx].cFileName);
                    if(NULL == info->name)
                    {
                        assert(NULL);
                    }
                    CTexAtlasManager::getSingleton().insert(index, info);
                }
            }
            LOGE("%s %d icons loading successfully!\n",respath,numFiles);
            Com_Free(NULL,filelist);
            return true;
        }
        
        /**
         *  纹理偏移图中插入图片
         *
         *  @param filepath 图片路径
         *  @param index    偏移图索引
         *
         *  @return 是否插入成功
         */
        bool GlobeFunction::InsertTextureToTextureAtlas(const char *filepath,const char *filename, int index)
        {
            if(NULL != filepath && NULL != filename)
            {
                char abspath[255] = {0};
                sprintf(abspath, "%s/%s.png",filepath,filename);
                if( Com_IsFileExist(abspath) )
                {
                    irr::video::IImage *bigimg =  CTexAtlasManager::getSingleton().getAtlasImage(index);
                    if(NULL == bigimg)
                    {
                        CTexAtlasManager::getSingleton().createAtlasMap(index,CmpPicName);
                    }
                    video::IImage *pimg =  mp::base::IMpSite::getSite()->getImageFactory()->load(abspath);
                    if(NULL == pimg )
                        return  false;
                    if(pimg->getColorFormat() == irr::video::ECF_R8G8B8)
                    {
                        printf("%s\n",filename);
                    }
                    TexUnitInfo *info = new TexUnitInfo;
                    info->img = pimg;
                    info->name = new char[strlen(filename)+ 1];
                    strcpy( info->name, filename);
                    CTexAtlasManager::getSingleton().insert(index, info);
                    return true;
                }
                else
                {
                    return false;
                }
            }
            else
            {
                return false;
            }
        }
        
        /**
         *  将内存地址转换成惟一id
         *
         *  @param pcString   内存地址
         *  @param ulHashType hash类型
         *  @param a_iLen     长度
         *
         *  @return id
         */
        u32 GlobeFunction::HashStringAlgo( u8* pcString, u32 ulHashType,u32 a_iLen )
        {
            u32 iTmpV;
            u8 *pcKey = (u8 *)pcString;
            u32 ulSeed1 = 0x7FED7FED;
            u32 ulSeed2 = 0xEEEEEEEE;
            
            while (/**pcKey != '\0' &&*/ a_iLen > 0)
            {
                iTmpV = (*pcKey);
                
                //根据type不同计算不同的hash值
                ulSeed1 = ((ulHashType << 8) + iTmpV) ^ (ulSeed1 + ulSeed2);
                ulSeed2 = iTmpV + ulSeed1 + ulSeed2 + (ulSeed2 << 5) + 3;
                
                pcKey++;
                a_iLen--;
            }
            
            return ulSeed1;
        }
        
        /**
         *  获取屏幕世界矩阵
         *
         *  @param pDrawState drawstate指针
         *  @param bound      屏幕范围
         */
        void GlobeFunction::BuildOthoDrawState(mp::draw::IDrawState *pDrawState, const Rect2di &bound)
        {
            Matrix4f projMat;
            projMat.buildProjectionMatrixOrthoLH((f32)bound.getWidth(),(f32)-bound.getHeight(),-1,1);
            projMat.setTranslation(Vector3df(-1,1,0));
            pDrawState->setProjectionTransform(projMat);
            Matrix4f viewMat;
            viewMat.setTranslation(Vector3df(0,0,0));
            pDrawState->setViewTransform(viewMat);
        }
        
        /**
         *  获取屏幕世界矩阵
         *
         *  @param matrix 屏幕世界矩阵
         *  @param bound  屏幕范围
         */
        void GlobeFunction::GetMatrixFromScreen( mp::Matrix4f &matrix, const Rect2di &bound)
        {
            Matrix4f projMat;
            projMat.buildProjectionMatrixOrthoLH((f32)bound.getWidth(),(f32)-bound.getHeight(),-1,1);
            projMat.setTranslation(Vector3df(-1,1,0));
            Matrix4f viewMat;
            viewMat.setTranslation(Vector3df(0,0,0));
            
            matrix = projMat * viewMat;
        }
        
        u32 GlobeFunction::UTF8StrToUnicode( const char* UTF8String, u32 UTF8StringLength, WCHAR* OutUnicodeString, u32 UnicodeStringBufferSize )
        {
            u32 UTF8Index = 0;
            u32 UniIndex = 0;
            
            while ( UTF8Index < UTF8StringLength )
            {
                unsigned char UTF8Char = UTF8String[UTF8Index];
                
                if ( UnicodeStringBufferSize != 0 && UniIndex >= UnicodeStringBufferSize )
                    break;
                
                if ((UTF8Char & 0x80) == 0)
                {
                    const u32 cUTF8CharRequire = 1;
                    
                    // UTF8字码不足
                    if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                        break;
                    
                    if ( OutUnicodeString )
                    {
                        WCHAR &WideChar = OutUnicodeString[UniIndex];
                        
                        WideChar = UTF8Char;
                    }
                    
                    UTF8Index++;
                    
                }
                else if((UTF8Char & 0xE0) == 0xC0)  ///< 110x-xxxx 10xx-xxxx
                {
                    const u32 cUTF8CharRequire = 2;
                    
                    // UTF8字码不足
                    if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                        break;
                    
                    if ( OutUnicodeString )
                    {
                        WCHAR &WideChar = OutUnicodeString[UniIndex];
                        WideChar  = (UTF8String[UTF8Index + 0] & 0x3F) << 6;
                        WideChar |= (UTF8String[UTF8Index + 1] & 0x3F);
                    }
                    
                    UTF8Index += cUTF8CharRequire;
                }
                else if((UTF8Char & 0xF0) == 0xE0)  ///< 1110-xxxx 10xx-xxxx 10xx-xxxx
                {
                    const u32 cUTF8CharRequire = 3;
                    
                    // UTF8字码不足
                    if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                        break;
                    
                    if ( OutUnicodeString )
                    {
                        WCHAR &WideChar = OutUnicodeString[UniIndex];
                        
                        WideChar  = (UTF8String[UTF8Index + 0] & 0x1F) << 12;
                        WideChar |= (UTF8String[UTF8Index + 1] & 0x3F) << 6;
                        WideChar |= (UTF8String[UTF8Index + 2] & 0x3F);
                    }
                    
                    
                    UTF8Index += cUTF8CharRequire;
                }
                else if((UTF8Char & 0xF8) == 0xF0)  ///< 1111-0xxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
                {
                    const u32 cUTF8CharRequire = 4;
                    
                    // UTF8字码不足
                    if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                        break;
                    
                    if ( OutUnicodeString )
                    {
                        WCHAR &WideChar = OutUnicodeString[UniIndex];
                        
                        WideChar  = (UTF8String[UTF8Index + 0] & 0x0F) << 18;
                        WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 12;
                        WideChar |= (UTF8String[UTF8Index + 2] & 0x3F) << 6;
                        WideChar |= (UTF8String[UTF8Index + 3] & 0x3F);
                    }
                    
                    UTF8Index += cUTF8CharRequire;
                }
                else ///< 1111-10xx 10xx-xxxx 10xx-xxxx 10xx-xxxx 10xx-xxxx
                {
                    const u32 cUTF8CharRequire = 5;
                    
                    // UTF8字码不足
                    if ( UTF8Index + cUTF8CharRequire > UTF8StringLength )
                        break;
                    
                    if ( OutUnicodeString )
                    {
                        WCHAR &WideChar = OutUnicodeString[UniIndex];
                        
                        WideChar  = (UTF8String[UTF8Index + 0] & 0x07) << 24;
                        WideChar  = (UTF8String[UTF8Index + 1] & 0x3F) << 18;
                        WideChar  = (UTF8String[UTF8Index + 2] & 0x3F) << 12;
                        WideChar |= (UTF8String[UTF8Index + 3] & 0x3F) << 6;
                        WideChar |= (UTF8String[UTF8Index + 4] & 0x3F);
                    }
                    
                    UTF8Index += cUTF8CharRequire;
                }
                
                
                UniIndex++;
            }
            
            return UniIndex;
        }
        
        void GlobeFunction::BuildPointSpriteMesh(const Vector3df &pos, PosUvArray &array, IndexArray &indices, float radius/* = 10*/,Vector3df offset /*= Vector3df(0,0,0)*/)
        {
            u32 sz = array.size();
            if( radius > 0.0 )
            {
                int len = 0;
                SDrawVertexUV1 item[4];
                item[0].pos = pos + Vector3df(-radius, -radius + offset.Y,len);
                item[1].pos = pos + Vector3df( radius, -radius + offset.Y,len);
                item[2].pos = pos + Vector3df( radius,  radius + offset.Y,len);
                item[3].pos = pos + Vector3df(-radius,  radius + offset.Y,len);
                
                item[0].uv1 = Vector2df( 0, 0 );
                item[1].uv1 = Vector2df( 1, 0 );
                item[2].uv1 = Vector2df( 1, 1 );
                item[3].uv1 = Vector2df( 0, 1 );
                
                
                array.push_back(item[0]);
                array.push_back(item[1]);
                array.push_back(item[2]);
                array.push_back(item[3]);
                
                indices.push_back(sz + 0);
                indices.push_back(sz + 1);
                indices.push_back(sz + 2);
                indices.push_back(sz + 0);
                indices.push_back(sz + 2);
                indices.push_back(sz + 3);
            }

        }
        
        void GlobeFunction::GPCSimpleTriangulate(const PosArray &inarray, mp::PosClrArray *outarray,u32 color /*= 0xff000000*/,  mp::IndexArray *idx/* = NULL*/)
        {
            if(inarray.empty())return;
            GPCSimpleTriangulate(inarray.const_pointer(), inarray.size() , outarray,color,idx);
        }

        void GlobeFunction::BoldLine(const PosArray &inarray, mp::PosClrArray *outarray,u32 color, mp::IndexArray *idx,float lineWidth, float zidx /*=0*/)
        {
            if(inarray.empty())return;
            BoldLine(inarray.const_pointer(), inarray.size() , outarray,color,idx,lineWidth,zidx);
        }
        void GlobeFunction::BoldLine( const Vector3df *pPoints, int nPoints, mp::PosClrArray *outarray,u32 color,  mp::IndexArray *idx, float lineWidth,float zidx /*=0*/)
        {
            if(NULL == pPoints || nPoints<2 || NULL == outarray || lineWidth < 1e-6)
                return;
            
            u32 i_idx = outarray->size();
            Vector2df a,b,aR,aL,bR,bL;
            Vector2df abP,abV,tmp;
            
            Vector3df LindS1[10];
            Vector3df LindE1[10];
            SDrawVertexColor vertexAcc;
            vertexAcc.color =SColor4f(color);
            for ( u32 i=1; i < nPoints; ++i )
            {
                b = ToVector2d(pPoints[i-1]);
                a = ToVector2d(pPoints[i]);
                abP = a-b;
                abP.normalize();
                abV = RotateBy90(abP);
                aR = a + abV * lineWidth;
                aL = a - abV * lineWidth;
                
                bR = b + abV * lineWidth;
                bL = b - abV * lineWidth;
                // 添加第一个点的左右点
                
                LindS1[0] = ToVector3d(b,zidx);
                LindS1[1] = ToVector3d(bR,zidx);
                LindS1[2] = ToVector3d(bL,zidx);
                LindS1[3] = ToVector3d(b - abP * lineWidth,zidx);
                LindS1[4] = ToVector3d(b + (-abP+abV)*0.707f * lineWidth,zidx);
                LindS1[5] = ToVector3d(b + (-abP-abV)*0.707f * lineWidth,zidx);
                LindS1[6] = ToVector3d(b + ((-abP+abV)*0.707f+abV)*0.541f *lineWidth,zidx);
                LindS1[7] = ToVector3d(b + ((-abP+abV)*0.707f-abP)*0.541f *lineWidth,zidx);
                LindS1[8] = ToVector3d(b + ((-abP-abV)*0.707f-abP)*0.541f *lineWidth,zidx);
                LindS1[9] = ToVector3d(b + ((-abP-abV)*0.707f-abV)*0.541f *lineWidth,zidx);
                
                // 添加最后一个点的左右点
                LindE1[0] = ToVector3d(a,zidx);
                LindE1[1] = ToVector3d(aR,zidx);
                LindE1[2] = ToVector3d(aL,zidx);
                LindE1[3] = ToVector3d(a + abP * lineWidth,zidx);
                LindE1[4] = ToVector3d(a + (abP-abV)*0.707f * lineWidth,zidx);
                LindE1[5] = ToVector3d(a + (abP+abV)*0.707f * lineWidth,zidx);
                LindE1[6] = ToVector3d(a + ((abP-abV)*0.707f-abV)*0.541f *lineWidth,zidx);
                LindE1[7] = ToVector3d(a + ((abP-abV)*0.707f+abP)*0.541f *lineWidth,zidx);
                LindE1[8] = ToVector3d(a + ((abP+abV)*0.707f+abP)*0.541f *lineWidth,zidx);
                LindE1[9] = ToVector3d(a + ((abP+abV)*0.707f+abV)*0.541f *lineWidth,zidx);
                
                for (int j=0;j<10;++j)
                {
                    vertexAcc.pos =  LindS1[j];outarray->push_back(vertexAcc);
                }
                idx->push_back(u32(i_idx + 1));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 6));
                idx->push_back(u32(i_idx + 6));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 4));
                idx->push_back(u32(i_idx + 4));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 7));
                idx->push_back(u32(i_idx + 7));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 3));
                idx->push_back(u32(i_idx + 3));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 8));
                idx->push_back(u32(i_idx + 8));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 5));
                idx->push_back(u32(i_idx + 5));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 9));
                idx->push_back(u32(i_idx + 9));
                idx->push_back(u32(i_idx + 0));
                idx->push_back(u32(i_idx + 2));
                
                u32 m_idx = i_idx+10;
                for (int j=0;j<10;++j)
                {
                    vertexAcc.pos =  LindE1[j];outarray->push_back(vertexAcc);
                    
                }
                idx->push_back(u32(m_idx + 1));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 9));
                idx->push_back(u32(m_idx + 9));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 5));
                idx->push_back(u32(m_idx + 5));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 8));
                idx->push_back(u32(m_idx + 8));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 3));
                idx->push_back(u32(m_idx + 3));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 7));
                idx->push_back(u32(m_idx + 7));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 4));
                idx->push_back(u32(m_idx + 4));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 6));
                idx->push_back(u32(m_idx + 6));
                idx->push_back(u32(m_idx + 0));
                idx->push_back(u32(m_idx + 2));
                
                idx->push_back(u32(i_idx + 1));
                idx->push_back(u32(i_idx + 12));
                idx->push_back(u32(i_idx + 11));
                idx->push_back(u32(i_idx + 1));
                idx->push_back(u32(i_idx + 2));
                idx->push_back(u32(i_idx + 12));
                i_idx += 20;
                
            }
        }

        void GlobeFunction::GPCSimpleTriangulate( const Vector3df *inarray, int posSize, mp::PosClrArray *outarray,u32 color /*= 0xff000000*/,  mp::IndexArray *idx /*= NULL*/,float zindex /*= 0.0*/)
        {
            if(NULL == outarray || NULL == inarray)return;
            
            bool hasidx = NULL != idx;
            
            gpc_polygon polygon;
            polygon.num_contours = 1;
            gpc_vertex_list vlist;
            polygon.hole = NULL;
            polygon.contour = &vlist;
            vlist.num_vertices = posSize;
            gpc_vertex *vertices = new gpc_vertex[vlist.num_vertices];
            vlist.vertex = vertices;
            for(int i = 0; i < vlist.num_vertices;++i)
            {
                vertices[i].x = inarray[i].X;
                vertices[i].y = inarray[i].Y;
            }
            gpc_tristrip tri;
            gpc_polygon_to_tristrip(&polygon, &tri);
            int index = 0;
            for(int i = 0;i < tri.num_strips;++i)
            {
                gpc_vertex_list *list = tri.strip + i;
                if(NULL != list)
                {
                    int vlen = list->num_vertices;
                    gpc_vertex *pvtx = list->vertex;
                    u32  basesz = outarray->size();
                    for(int j = 0; j < vlen; ++j)
                    {
                        SDrawVertexColor item;
                        item.pos = Vector3df(pvtx[j].x,pvtx[j].y,zindex);
                        item.color = SColor4i(color);
                        outarray->push_back(item);
                        if(hasidx && j > 1)
                        {
                            idx->push_back(basesz + index + 0);
                            idx->push_back(basesz + index + 1);
                            idx->push_back(basesz + index + 2);
                            ++index;
                        }
                    }
                    index = 0;
                }
                
            }
            delete [] vertices;
            gpc_free_tristrip(&tri);
        }
        
        //创建gpc顶点链表
        static void createGpcList(gpc_vertex_list *vlist,const Vector3df *inarray,int posSize)
        {
            vlist->num_vertices = posSize;
            gpc_vertex *vertices = NULL;
            GPC_MALLOC(vertices, vlist->num_vertices, "vertex create", gpc_vertex);
            vlist->vertex = vertices;
            for(int i = 0; i < vlist->num_vertices;++i)
            {
                vertices[i].x = inarray[i].X;
                vertices[i].y = inarray[i].Y;
            }
        }
        //销毁gpc顶点链表
        static inline void destroyGpclist( gpc_vertex_list *vlist,  int sz)
        {//资源释放
            for(int i = 0; i < sz; ++i)
            {
                GPC_FREE(vlist[i].vertex);
            }
            GPC_FREE(vlist);
        }
        
        void GlobeFunction::GPCComplexTraiangulate(const std::vector<PosArray> &inarray, mp::PosClrArray *outarray, mp::IndexArray *idx,u32 color /*=0xffff0000*/)
        {
            const int vLen = inarray.size();
            bool hasidx = NULL != idx;
            gpc_polygon  polygon;
            polygon.num_contours = vLen;
            gpc_vertex_list *vlist;
            
            GPC_MALLOC(vlist, vLen, "", gpc_vertex_list);
            polygon.hole = NULL;
            polygon.contour = vlist;
            
            for(int i = 0; i < vLen ;++i)
            {
                createGpcList(&vlist[i], inarray[i].const_pointer(), inarray[i].size());
            }
            
            gpc_tristrip tri;
            gpc_polygon_to_tristrip(&polygon, &tri);
            int index = 0;
            for(int i = 0;i < tri.num_strips;++i)
            {
                gpc_vertex_list *list = tri.strip + i;
                if(NULL != list)
                {
                    int vlen = list->num_vertices;
                    gpc_vertex *pvtx = list->vertex;
                    u32  basesz = outarray->size();
                    for(int j = 0; j < vlen; ++j)
                    {
                        SDrawVertexColor item;
                        item.pos = Vector3df(pvtx[j].x,pvtx[j].y,0);
                        item.color = SColor4i(color);
                        outarray->push_back(item);
                        if(hasidx && j > 1)
                        {
                            idx->push_back(basesz + index + 0);
                            idx->push_back(basesz + index + 1);
                            idx->push_back(basesz + index + 2);
                            ++index;
                        }
                    }
                    index = 0;
                }
                
            }
            destroyGpclist(vlist, vLen);
            gpc_free_tristrip(&tri);
        }
        
        void GlobeFunction::CalcBoxSize(const Box2dd &box)
        {
            mp::Vector2dd pt = box.MinEdge;
            mp::Vector3dd minwrldpt = mp::earth::IEarthModel::getInstance()->computeAbsolutePosition(mp::Vector3dd(pt.Y,pt.X,0));
            pt = box.MaxEdge;
            mp::Vector3dd maxwrldpt = mp::earth::IEarthModel::getInstance()->computeAbsolutePosition(mp::Vector3dd(pt.Y,pt.X,0));
            double minx = minwrldpt.X;
            double maxx = maxwrldpt.X;
            double miny = minwrldpt.Y;
            double maxy = maxwrldpt.Y;
            
            printf("X length is %f\n",fabs(maxx - minx));
            printf("Y length is %f\n",fabs(maxy - miny));
        }
        
        Box2dd GlobeFunction::GetBoundBox(const GPosition* positionArray,int num)
        {
            Box2dd box(0,0,0,0);
            if(NULL == positionArray)
                return box;
            GPosition gmax(0,0,0);
            GPosition gmin(180,180,0);
            for (int i = 0; i < num; ++i)
            {
                if(positionArray[i].X <= gmin.X)
                    gmin.X = positionArray[i].X;
                if(positionArray[i].X >= gmax.X)
                    gmax.X = positionArray[i].X;
                if(positionArray[i].Y <= gmin.Y)
                    gmin.Y = positionArray[i].Y;
                if(positionArray[i].Y >= gmax.Y)
                    gmax.Y = positionArray[i].Y;
            }
            box = Box2dd(gmin.X,gmin.Y,gmax.X,gmax.Y);
            return box;
        }
        
       
        
       
        INITGLOBEVAR(LanguageCode       , 0                     , u16);
        INITGLOBEVAR(DpiRate            , (2.54 / 96)           , float);
        INITGLOBEVAR(ScreenMatrix       , mp::Matrix4f()        , mp::Matrix4f);
        INITGLOBEVAR(ScaleCursorPos     , mp::Vector2di(0,0)    , mp::Vector2di);
        INITGLOBEVAR(ScaleCursorVisible , true                  , bool);
        INITGLOBEVAR(DebugInfoVisible   , true                  , bool);
        INITGLOBEVAR(SleepTime          , 3000                  , u32);
        INITGLOBEVAR(ThreadSleep        , true                  , bool);
        INITGLOBEVAR(ThreadSleepInterval, 200                   , int)
    }
}
