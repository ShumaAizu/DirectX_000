//=============================================================================
//
//	�J�������W���� [camera.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "main.h"

//*****************************************************************************
// �v���g�^�C�v�錾
//*****************************************************************************
void InitCamera(void);
void UninitCamera(void);
void UpdateCamera(void);
void DrawCamera(void);
D3DXVECTOR3* GetCamera(void);

#endif		// _CAMERA_H_