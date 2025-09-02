//=============================================================================
//
//	�e�̏��� [bullet.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "player.h"
#include "bullet.h"
#include "explosion.h"
#include "enemy.h"
#include "effect.h"
#include "option.h"
#include "particle.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_BULLET		(2048)			// �e�̍ő吔

//*****************************************************************************
// �z�[�~���O�^�[�Q�b�g�̍\���̒�`
//*****************************************************************************
typedef struct
{
	D3DXVECTOR3 pos;
	float fVector;
	float fAngle;
}Target;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureBullet = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBullet = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Bullet g_aBullet[MAX_BULLET];							// �e�̏��
int g_nCounterAnimBullet;								// �A�j���[�V�����J�E���^�[
int g_nPatternAnimBullet;								// �A�j���[�V�����p�^�[��No.

//=============================================================================
//	�e�̏���������
//=============================================================================
void InitBullet(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	int nCntBullet = 0;		// �J�E���g�p�ϐ�

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bullet000.png",
		&g_pTextureBullet);

	// ������
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBullet[nCntBullet].target = NULL;
		g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
		g_aBullet[nCntBullet].nLife = 30;
		g_aBullet[nCntBullet].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���

		// �Ίp���̒������Z�o����
		g_aBullet[nCntBullet].fLength = sqrtf(BULLET_SIZEX * BULLET_SIZEX + BULLET_SIZEY * BULLET_SIZEY) * 0.5f;

		// �Ίp���̊p�x���Z�o����
		g_aBullet[nCntBullet].fAngle = atan2f(BULLET_SIZEX, BULLET_SIZEY);
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BULLET,		// �e�̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBullet,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void * *)&pVtx, 0);

	// ���_���̐ݒ�
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZEX, g_aBullet[nCntBullet].pos.y - BULLET_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZEX, g_aBullet[nCntBullet].pos.y - BULLET_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x - BULLET_SIZEX, g_aBullet[nCntBullet].pos.y + BULLET_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aBullet[nCntBullet].pos.x + BULLET_SIZEX, g_aBullet[nCntBullet].pos.y + BULLET_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	�e�̏I������
//=============================================================================
void UninitBullet(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureBullet != NULL)
	{
		g_pTextureBullet->Release();
		g_pTextureBullet = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBullet != NULL)
	{
		g_pVtxBuffBullet->Release();
		g_pVtxBuffBullet = NULL;
	}
}

//=============================================================================
//	�e�̕`�揈��
//=============================================================================
void DrawBullet(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBullet, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureBullet);

	// �g�p����
	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e���g�p����Ă���
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBullet * 4, 2);
		}
	}
}

//=============================================================================
//	�e�̍X�V����
//=============================================================================
void UpdateBullet(void)
{
	int nCntBullet = 0;

	Bullet* pBullet = &g_aBullet[0];

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// �g�p����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++, pBullet++)
	{
		if (pBullet->bUse == true)
		{// �e���g�p����Ă���

			if (pBullet->type == BULLETTYPE_PLAYER)
			{// �v���C���[�̒e
				// �G�t�F�N�g�̐ݒ�
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.25f, 0.1f, 0.25f, 1.0f), 25.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.75f, 0.1, 0.75f, 1.0f), 45.0f, 50);
				SetParticle(pBullet->pos, D3DXCOLOR(1.0f, 0.0f, 1.0f, 1.0f), 30.0f, 3);
				// �G�Ƃ̓����蔻��
				CollisionEnemy(pBullet);
			}
			else if (pBullet->type == BULLETTYPE_ENEMY)
			{// �G�̒e
				// �G�t�F�N�g�̐ݒ�
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 15.0f, 50);
				SetEffect(pBullet->pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.75f, 0.1f, 0.1f, 1.0f), 30.0f, 50);
				// �v���C���[�Ƃ̓����蔻��
				CollisionPlayer(pBullet);
				CollisionOption(pBullet);
			}

			switch (pBullet->shottype)
			{
			case SHOTTYPE_HOMING:
				Enemy* pEnemy = GetEnemy();

				if ((pEnemy + pBullet->target)->bUse == false)
				{
					pBullet->shottype = SHOTTYPE_NORMAL;
				}
				else
				{
					float fAngle = atan2f((pEnemy + pBullet->target)->pos.x - pBullet->pos.x, (pEnemy + pBullet->target)->pos.y - pBullet->pos.y);

					pBullet->move.x = sinf(fAngle) * 15.0f;
					pBullet->move.y = cosf(fAngle) * 15.0f;

					pBullet->move.x += (0.0f - pBullet->move.x) * 0.3f;
					pBullet->move.y += (0.0f - pBullet->move.y) * 0.3f;
				}

				break;
			}

			// �ʒu�̍X�V
			pBullet->pos += pBullet->move;

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z + D3DX_PI + pBullet->fAngle) * pBullet->fLength;
			pVtx[0].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z + D3DX_PI + pBullet->fAngle) * pBullet->fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z - D3DX_PI - pBullet->fAngle) * pBullet->fLength;
			pVtx[1].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z - D3DX_PI - pBullet->fAngle) * pBullet->fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z - pBullet->fAngle) * pBullet->fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBullet->pos.x - pCameraPos->x + sinf(pBullet->rot.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.y = pBullet->pos.y - pCameraPos->y + cosf(pBullet->rot.z + pBullet->fAngle) * pBullet->fLength;
			pVtx[3].pos.z = 0.0f;

			//// �g�p����
			//if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x < 0 ||
			//	g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
			//{// �����e����ʊO�ɏo����
			//	g_aBullet[nCntBullet].bUse = false;		// �e���g�p���Ă��Ȃ���Ԃɂ���
			//}

			pBullet->nLife--;				// ���������

			if (pBullet->nLife < 0)
			{//�����������s������
				SetExplosion(pBullet->pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				pBullet->bUse = false;		// �e���g�p���Ă��Ȃ���Ԃɂ���
			}

			if (pBullet->bUse == false)
			{// �e���g�p���Ă��Ȃ���ԂɂȂ�����
				// ������
				pBullet->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				pBullet->target = NULL;
				pBullet->nLife = 30;
			}
		}

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	�G�̒e�̐ݒ�
//=============================================================================
void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type,SHOTTYPE shottype, float fAngleE_P)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p���Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].shottype = shottype;

			switch (g_aBullet[nCntBullet].shottype)
			{
			case SHOTTYPE_HOMING:				// �z�[�~���O�e
				float fLength = NULL;			// �Ίp���̒���
				float fLengthLast = 250.0f;		// ��ԒZ���Ίp���̒���
				Enemy* pEnemy = GetEnemy();		// �G�̏��擾
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false)
					{
						continue;
					}

					// �Ίp���̒����Z�o
					fLength = SQRTF((pEnemy->pos.x - g_aBullet[nCntBullet].pos.x), (pEnemy->pos.y - g_aBullet[nCntBullet].pos.y));
					if (fLength < fLengthLast)
					{// ������ԒZ���Ίp���̒������X�V���ꂽ��
						fLengthLast = fLength;
						g_aBullet[nCntBullet].target = nCntEnemy;
					}
				}

				if (fLengthLast >= 250.0f)
				{
					g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
				}
			}

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move.x = sinf(fAngleE_P) * fmove;
			g_aBullet[nCntBullet].move.y = cosf(fAngleE_P) * fmove;

			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;		// �e���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();
}

//=============================================================================
//	�v���C���[�̒e�̐ݒ�
//=============================================================================
void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type, SHOTTYPE shottype)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p���Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			g_aBullet[nCntBullet].shottype = shottype;

			switch (g_aBullet[nCntBullet].shottype)
			{
			case SHOTTYPE_HOMING:				// �z�[�~���O�e
				float fLength = NULL;			// �Ίp���̒���
				float fLengthLast = 250.0f;		// ��ԒZ���Ίp���̒���
				Enemy* pEnemy = GetEnemy();		// �G�̏��擾
				for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
				{
					if (pEnemy->bUse == false)
					{
						continue;
					}

					// �Ίp���̒����Z�o
					fLength = SQRTF((pEnemy->pos.x - g_aBullet[nCntBullet].pos.x), (pEnemy->pos.y - g_aBullet[nCntBullet].pos.y));
					if (fLength < fLengthLast)
					{// ������ԒZ���Ίp���̒������X�V���ꂽ��
						fLengthLast = fLength;
						g_aBullet[nCntBullet].target = nCntEnemy;
					}
				}

				if (fLengthLast >= 250.0f)
				{
					g_aBullet[nCntBullet].shottype = SHOTTYPE_NORMAL;
				}
			}

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x - pCameraPos->x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y - pCameraPos->y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.z = 0.0f;

			g_aBullet[nCntBullet].move = move;

			g_aBullet[nCntBullet].nLife = nLife;
			g_aBullet[nCntBullet].type = type;
			g_aBullet[nCntBullet].bUse = true;		// �e���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBullet->Unlock();

}
//=============================================================================
//	�G�Ƃ̓����蔻��
//=============================================================================
void CollisionEnemy(Bullet *pBullet)
{
	// �G���̎擾
	Enemy *pEnemy = GetEnemy();

	// �g�p����
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// �G���g�p����Ă���
			if (pBullet->pos.x >= pEnemy->pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
				pBullet->pos.y >= pEnemy->pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
				pBullet->pos.x <= pEnemy->pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
				pBullet->pos.y <= pEnemy->pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2))
			{// �����e���G�ɂ������Ă�����
				// �G�̃q�b�g����
				HitEnemy(nCntEnemy, 1);
				pBullet->bUse = false;
			}
		}
	}
}

//=============================================================================
//	�v���C���[�Ƃ̓����蔻��
//=============================================================================
void CollisionPlayer(Bullet* pBullet)
{
	// �v���C���[���̎擾
	Player *pPlayer = GetPlayer();

	if (pBullet->pos.x >= pPlayer->pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
		pBullet->pos.y >= pPlayer->pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
		pBullet->pos.x <= pPlayer->pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
		pBullet->pos.y <= pPlayer->pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2) &&
		pPlayer->state == PLAYERSTATE_NORMAL)
	{// �����e���v���C���[�ɂ������Ă�����
		// �G�̃q�b�g����
		HitPlayer(1);
		pBullet->bUse = false;
	}
}

//=============================================================================
//	�I�v�V�����Ƃ̓����蔻��
//=============================================================================
void CollisionOption(Bullet* pBullet)
{
	// �v���C���[���̎擾
	Option* pOption = GetOption();

	// �g�p����
	for (int nCntEnemy = 0; nCntEnemy < MAX_OPTION; nCntEnemy++, pOption++)
	{
		if (pOption->bUse == true)
		{// �I�v�V�������g�p����Ă���
			if (pBullet->pos.x >= pOption->pos.x - ENEMY_SIZEX - (BULLET_SIZEX / 2) &&
				pBullet->pos.y >= pOption->pos.y - ENEMY_SIZEY - (BULLET_SIZEY / 2) &&
				pBullet->pos.x <= pOption->pos.x + ENEMY_SIZEX + (BULLET_SIZEX / 2) &&
				pBullet->pos.y <= pOption->pos.y + ENEMY_SIZEY + (BULLET_SIZEY / 2) &&
				pOption->state == SUBOPTIONSTATE_BARRIER)
			{// �����e���I�v�V�����ɂ������Ă�����
				// �I�v�V�����̃q�b�g����
				HitOption(nCntEnemy);
				pBullet->bUse = false;
			}
		}
	}
}