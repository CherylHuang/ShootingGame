#include "CCamera.h"

CCamera* CCamera::_pCamera = nullptr;

CCamera::CCamera() {
	_type = Type::PERSPECTIVE;
	_bViewDirty = true;
	_bProjectionDirty = true;
}

void CCamera::destroyInstance()
{
	CC_SAFE_DELETE(_pCamera);
}

CCamera::~CCamera()
{

}

// 預設是以透視投影，view 在 (0,0,0) 看向 -Z 方向
// fov = 60度
CCamera* CCamera::create()
{
	if (_pCamera == nullptr) {
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void  CCamera::initDefault()
{
	// 設定預設鏡頭的位置
	_viewPosition = vec4(0, 0, 0, 1.0f);
	_lookAt = vec4(0, 0, -1.0f, 1.0f);
	_upVector = vec4(0, 1.0f, 0, 1.0f);
	// 產生 View Matrix
	_view = LookAt(_viewPosition, _lookAt, _upVector);

	// 以常用的大小開啟透視投影作為預設的鏡頭投影方式
	_projection = Perspective(60.0, 1.0f, 1.0, 1000.0);
	_bViewDirty = true;
	_bProjectionDirty = true;
	_viewProjection = _projection * _view;	// 可以先不用計算
}

CCamera* CCamera::getInstance()
{
	if ( _pCamera == nullptr ){
		_pCamera = new (std::nothrow) CCamera;
		_pCamera->initDefault();
	}
	return _pCamera;
}

void CCamera::updatePerspective(float fieldOfView, float aspectRatio, float nearPlane, float farPlane)
{
	_projection = Perspective(fieldOfView, aspectRatio, nearPlane, farPlane);
	_bProjectionDirty = true;
	_type = Type::PERSPECTIVE;
	_viewProjection = _projection * _view; // 同時更新  viewProjection matrix (可以先不用計算)
}

void CCamera::updateOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane)
{
	_projection = Ortho(left, right, bottom, top, nearPlane, farPlane);
	_bProjectionDirty = true;
	_type = Type::ORTHOGRAPHIC;
	_viewProjection = _projection * _view; // 同時更新  viewProjection matrix (可以先不用計算)
}

void CCamera::updateViewPosition(vec4 &vp)
{
	// 產生 View Matrix
	_viewPosition = vp;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // 可以先不用計算
}
void CCamera::updateLookAt(vec4 &at)
{
	// 產生 View Matrix
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view;  // 可以先不用計算
}

void CCamera::updateViewLookAt(vec4 &vp, vec4 &at)
{
	// 產生 View Matrix
	_viewPosition = vp;
	_lookAt = at;
	_view = LookAt(_viewPosition, _lookAt, _upVector);
	_bViewDirty = true;
	_viewProjection = _projection * _view; // 可以先不用計算
}

const mat4& CCamera::getProjectionMatrix(bool &bProj) const
{
	bProj = _bProjectionDirty;
	if (_bProjectionDirty) _bProjectionDirty = false; // 重新取得 就設定成 false
	return _projection;
}
const mat4& CCamera::getViewMatrix(bool &bView) const
{
	bView = _bViewDirty;
	if (_bViewDirty) _bViewDirty = false; // 重新取得 就設定成 false
	return _view;
}

const mat4& CCamera::getViewProjectionMatrix() const
{
	return _viewProjection;
}

CCamera::Type CCamera::getProjectionType()
{
	return _type;
}