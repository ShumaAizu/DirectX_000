//=============================================================================
//
//	弾の処理 [bullet.h]
//	Author : SHUMA AIZU
// 
//=============================================================================

#ifndef _BULLET_H_
#define _BULLET_H_

#include "main.h"

//*****************************************************************************
// マクロ定義
//*****************************************************************************
#define BULLET_SIZEX		(15)			// 弾のサイズX
#define BULLET_SIZEY		(15)			// 弾のサイズY
#define BULLET_MOVE			(15.0f)			// 弾の移動量


//*****************************************************************************
// 弾の種類
//*****************************************************************************
typedef enum
{
	BULLETTYPE_PLAYER = 0,				// プレイヤーの弾
	BULLETTYPE_ENEMY,					// 敵の弾
	BULLETTYPE_MAX						// 総数
}BULLETTYPE;

//*****************************************************************************
// 弾の構造体定義
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;					// 位置
	D3DXVECTOR3 move;					// 移動量
	D3DXVECTOR3 rot;					// 向き
	BULLETTYPE type;					// 弾の種類
	float fLength;						// 対角線の長さ
	float fAngle;						// 対角線の角度
	int nLife;							// 寿命
	bool bUse;							// 使用しているかどうか
}Bullet;

//*****************************************************************************
// プロトタイプ宣言
//*****************************************************************************
void InitBullet(void);
void UninitBullet(void);
void UpdateBullet(void);
void DrawBullet(void);
void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type);
void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type, float fAngleE_P);
void CollisionEnemy(Bullet *pBullet);
void CollisionPlayer(Bullet* pBullet);
void CollisionOption(Bullet* pBullet);

#endif
