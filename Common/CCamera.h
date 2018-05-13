#ifndef CCAMERA_H
#define CCAMERA_H

#define CC_SAFE_DELETE(p)           do { delete (p); (p) = nullptr; } while(0)
#define CC_SAFE_DELETE_ARRAY(p)     do { if(p) { delete[] (p); (p) = nullptr; } } while(0)

#include "../header/Angel.h"

class CCamera
{
public:
	// enum class ：strongly typed enumerations, Strongly typed enums are a new kind of enum
	//    The use of the word class is meant to indicate that each enum type really is different and not comparable to other enum types. 
	//    Each enum value is scoped within the name of the enum class. In other words, to access the enum values, you must write:
	//    Type::PERSPECTIVE
	enum class Type  
	{
		PERSPECTIVE = 1,
		ORTHOGRAPHIC = 2
	};

	void updatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane);
	void updateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);
	void updateViewLookAt(vec4 &vp, vec4 &at);
	void updateViewPosition(vec4 &vp);
	void updateLookAt(vec4 &at);

	CCamera::Type getProjectionType();	// 取得目前的投影型態

	const mat4& getProjectionMatrix(bool &bProj) const;	// 取得投影矩陣
	const mat4& getViewMatrix(bool &bView) const;			// 取得 View Matrix
	const mat4& getViewProjectionMatrix() const;	// 取的 VP 矩陣

	static CCamera* create();		// 建立 CCamera 的實體
	static CCamera *getInstance();	// 取得  CCamera 的實體

	void destroyInstance();		// 釋放取得資源

protected:
	CCamera();		// 避免被誤用
	~CCamera();
	void initDefault();


	// For Camera (View Matrix)
	vec4 _viewPosition;
	vec4 _lookAt;
	vec4 _upVector;

	mat4 _projection;
	mutable mat4 _view;
	mutable mat4 _viewInv;
	mutable mat4 _viewProjection;
	CCamera::Type _type;
	mutable bool _bViewDirty;		// View 矩陣內容更新過 必須重新取得
	mutable bool _bProjectionDirty;	// Projection 矩陣內容更新過 必須重新取得

	static CCamera* _pCamera;
};

#endif

