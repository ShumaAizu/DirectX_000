//=============================================================================
//
//	�G�̏��� [bossparts.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "enemy.h"
#include "bossparts.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "life.h"
#include "camera.h"
#include "particle.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_BOSSPARTS		(3)			// �G�̎��
#define BOSSPARTS_SCORE		(500)		// �G���j���̊l���X�R�A

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureBossParts[NUM_BOSSPARTS] = {};		// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffBossParts = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
BossParts g_aBossParts[MAX_BOSSPARTS];								// �G�̏��

//====================================
//	�G�̏���������
//====================================
void InitBossParts(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntBossParts = 0;
	D3DXVECTOR3* pCameraPos = GetCamera();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts000.png",
		&g_apTextureBossParts[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts001.png",
		&g_apTextureBossParts[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\bossparts002.png",
		&g_apTextureBossParts[2]);

	// ������
	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		g_aBossParts[nCntBossParts].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossParts[nCntBossParts].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aBossParts[nCntBossParts].fRadius = 0.0f;
		g_aBossParts[nCntBossParts].bDisp = false;
		g_aBossParts[nCntBossParts].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
		g_aBossParts[nCntBossParts].bBlinking = false;
		g_aBossParts[nCntBossParts].fMove = 0.0f;
		g_aBossParts[nCntBossParts].nTimeLine = -1;
		g_aBossParts[nCntBossParts].nScore = 0;
		g_aBossParts[nCntBossParts].nLife = 3;
		g_aBossParts[nCntBossParts].nCounterAnim = 0;
		g_aBossParts[nCntBossParts].nPatternAnim = 0;
		g_aBossParts[nCntBossParts].nCounterState = 0;
		g_aBossParts[nCntBossParts].nCounterAttack = 60;
		g_aBossParts[nCntBossParts].nUseCounter = 600;
		g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_NORMAL;
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_BOSSPARTS,		// �G�̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffBossParts,
		NULL);

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		float fAngle = atan2f(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);
		float fRadius = SQRTF(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
		pVtx[0].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
		pVtx[1].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
		pVtx[3].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
		pVtx[3].pos.z = 0.0f;

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

		if (nCntBossParts % 2 != 0)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}
		else
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(1.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.0f, 1.0f);
		}

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	�G�̏I������
//=============================================================================
void UninitBossParts(void)
{
	int nCntTex;
	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < NUM_BOSSPARTS; nCntTex++)
	{
		if (g_apTextureBossParts[nCntTex] != NULL)
		{
			// �e�N�X�`���̔j��
			g_apTextureBossParts[nCntTex]->Release();
			g_apTextureBossParts[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffBossParts != NULL)
	{
		g_pVtxBuffBossParts->Release();
		g_pVtxBuffBossParts = NULL;
	}
}

//=============================================================================
//	�G�̕`�揈��
//=============================================================================
void DrawBossParts(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffBossParts, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		if (g_aBossParts[nCntBossParts].bUse == true && g_aBossParts[nCntBossParts].bDisp == true)
		{// �G���g�p����Ă���
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureBossParts[g_aBossParts[nCntBossParts].type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntBossParts * 4, 2);
		}
	}
}

//=============================================================================
//	�G�̍X�V����
//=============================================================================
void UpdateBossParts(void)
{
	int nCntBossParts = 0;

	BossParts* pBossParts = &g_aBossParts[0];

	Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾

	D3DXVECTOR3* pCameraPos = GetCamera();

	Enemy* pBossEnemy = GetBOSSEnemy();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	// �g�p����
	for (nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++, pVtx += 4, pBossParts++)
	{
		if (pBossParts->bUse == true)
		{// �����G���g�p����Ă�����
			Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾
			float fAngleE_P = atan2f(pPlayer->pos.x - pBossParts->pos.x,		// �v���C���[�ƓG�Ƃ̊p�x�Z�o
				pPlayer->pos.y - pBossParts->pos.y);

			float fAngle = atan2f(pBossParts->fRadius, pBossParts->fRadius);
			float fRadius = SQRTF(pBossParts->fRadius, pBossParts->fRadius);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + fAngle) * fRadius;
			pVtx[0].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + fAngle) * fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z - fAngle) * fRadius;
			pVtx[1].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z - fAngle) * fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pBossParts->pos.x - pCameraPos->x + sinf(pBossParts->rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.y = pBossParts->pos.y - pCameraPos->y + cosf(pBossParts->rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.z = 0.0f;

			// ============== //
			// �G�̏��		  //
			// ============== //
			switch (pBossParts->state)
			{// ��ԃ`�F�b�N
				// �o�����
			case BOSSPARTSSTATE_APPEAR:
				SetRainbowParticle(pBossParts->pos, 2500.0f, 10, D3DX_PI, -D3DX_PI);	// �p�[�e�B�N����ݒ�
				pBossParts->nCounterState--;			// �J�E���g�����炷
				if (pBossParts->nCounterState <= 0)
				{// �J�E���g��0�ȉ��ɂȂ����Ȃ�
					pBossParts->bDisp = true;				// �`��
					pBossParts->nCounterState = 0;			// �J�E���^�[���Z�b�g
					pBossParts->state = BOSSPARTSSTATE_NORMAL;	// ��Ԃ�ʏ��
				}
				continue;
				break;

				// �ʏ���
			case BOSSPARTSSTATE_NORMAL:

				break;

				// �_���[�W���
			case BOSSPARTSSTATE_DAMAGE:
				pBossParts->nCounterState--;		// ��Ԏ������Ԃ����炷
				if (pBossParts->nCounterState <= 0)
				{// ��Ԏ������Ԃ��Ȃ��Ȃ�����
					pBossParts->state = BOSSPARTSSTATE_NORMAL;
					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

				}
				break;

			case BOSSPARTSSTATE_WAIT:
				pBossParts->state = BOSSPARTSSTATE_APPEAR;
				pBossParts->nCounterState = 60;
				break;
			}

			switch (pBossParts->type)
			{
			case BOSSPARTSTYPE_WING:
				if (nCntBossParts % 2 == 0)
				{
					SetParticle(D3DXVECTOR3(pBossParts->pos.x - 45.0f, pBossParts->pos.y + 5.0f, 0.0f), D3DXCOLOR(0.95f, 1.0f, 0.95f, 1.0f), 2500.0f, 5, (0.15f * D3DX_PI), (-0.85f * D3DX_PI));
				}
				else
				{
					SetParticle(D3DXVECTOR3(pBossParts->pos.x + 45.0f, pBossParts->pos.y + 5.0f, 0.0f), D3DXCOLOR(0.95f, 1.0f, 0.95f, 1.0f), 2500.0f, 5, (0.85f * D3DX_PI), (-0.15f * D3DX_PI));
				}
				pBossParts->nCounterAttack--;
				if (pBossParts->nCounterAttack <= 0)
				{
					if (nCntBossParts % 2 == 0)
					{
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.75f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.60f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (-0.45f * D3DX_PI));
					}
					else
					{
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.75f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.60f * D3DX_PI));
						SetEnemyBullet(pBossParts->pos, 25.0f, 250, BULLETTYPE_WIND, SHOTTYPE_WING, (0.45f * D3DX_PI));
					}
					pBossParts->nCounterAttack = 180;
				}


				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				break;

			case BOSSPARTSTYPE_HAND:
				SetParticle(pBossParts->pos, D3DXCOLOR(0.5f, 0.25f, 0.0f, 1.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);

				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;

				break;

			case BOSSPARTSTYPE_FLARE:
				SetParticle(D3DXVECTOR3(pBossParts->pos.x, pBossParts->pos.y - 25.0f, 0.0f), D3DXCOLOR(0.6f, 0.9f, 1.0f, 1.0f), 2500.0f, 5, (0.25f * D3DX_PI), (-0.25f * D3DX_PI));


				pBossParts->pos.x += sinf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				pBossParts->pos.y += cosf(pBossEnemy->rot.z * D3DX_PI) * pBossEnemy->fMove;
				break;

			}

			if (pPlayer->state == PLAYERSTATE_DEATH)
			{// �v���C���[�����S��ԂȂ�F��߂�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}

			// �g�p����
			if (pBossParts->pos.x <= 0 || pBossParts->pos.x >= WARLD_WIDTH ||
				pBossParts->pos.y <= 0 || pBossParts->pos.y >= WARLD_HEIGHT ||
				pBossParts->nUseCounter <= 0)
			{// ��ʊO�ɏo�� / �J�E���^�[�����؂���
				ResetBossParts(pBossParts);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossParts->Unlock();
}
//=============================================================================
//	�G�̐ݒ�
//=============================================================================
void SetBossParts(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++)
	{
		if (g_aBossParts[nCntBossParts].bUse == false && g_aBossParts[nCntBossParts].state != BOSSPARTSSTATE_WAIT)
		{// �G���g�p���Ă��Ȃ�
			g_aBossParts[nCntBossParts].pos = pos;
			g_aBossParts[nCntBossParts].rot = rot;
			g_aBossParts[nCntBossParts].type = (BOSSPARTSTYPE)nType;
			g_aBossParts[nCntBossParts].fRadius = fRadius;
			g_aBossParts[nCntBossParts].fMove = fMove;
			g_aBossParts[nCntBossParts].nLife = nLife;
			g_aBossParts[nCntBossParts].nTimeLine = nTimeLine;
			g_aBossParts[nCntBossParts].nScore = nScore;
			g_aBossParts[nCntBossParts].nCounterAttack = 60;
			g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_WAIT;

			float fAngle = atan2f(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);
			float fRadius = SQRTF(g_aBossParts[nCntBossParts].fRadius, g_aBossParts[nCntBossParts].fRadius);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
			pVtx[0].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + fAngle) * fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
			pVtx[1].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z - fAngle) * fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBossParts[nCntBossParts].pos.x - pCameraPos->x + sinf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.y = g_aBossParts[nCntBossParts].pos.y - pCameraPos->y + cosf(g_aBossParts[nCntBossParts].rot.z + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.z = 0.0f;
			break;		// ������for���𔲂���
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	�G�̎擾
//=============================================================================
BossParts* GetBossParts(void)
{
	return &g_aBossParts[0];		// �G�̏��̐擪�A�h���X��Ԃ�
}

//=============================================================================
//	�G�̃q�b�g����
//=============================================================================
void HitBossParts(int nCntBossParts, int nDamage)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBossParts->Lock(0, 0, (void**)&pVtx, 0);

	g_aBossParts[nCntBossParts].nLife -= nDamage;		// �G�̗̑͂����炷
	if (g_aBossParts[nCntBossParts].nLife <= 0)
	{// �G�̗̑͂��Ȃ��Ȃ�����
		//SetExplosion(g_aBossParts[nCntBossParts].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//SetParticle(g_aBossParts[nCntBossParts].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f, 10);
		SetRainbowParticle(g_aBossParts[nCntBossParts].pos, 3000.0f, 20, D3DX_PI, -D3DX_PI);
		AddScore(g_aBossParts[nCntBossParts].nScore);

		ResetBossParts(&g_aBossParts[nCntBossParts]);

		pVtx += (nCntBossParts * 4);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		PlaySound(SOUND_LABEL_SE_EXPLOSION001);
	}
	else
	{// �G�̗̑͂��c���Ă�����
		g_aBossParts[nCntBossParts].state = BOSSPARTSSTATE_DAMAGE;		// �_���[�W��Ԃɂ���
		g_aBossParts[nCntBossParts].nCounterState = 12;

		PlaySound(SOUND_LABEL_SE_HIT001);

		pVtx += (nCntBossParts * 4);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffBossParts->Unlock();
}

//=============================================================================
//	�G�̃��Z�b�g����
//=============================================================================
void ResetBossParts(BossParts* pBossParts)
{
	// ������
	pBossParts->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pBossParts->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pBossParts->fRadius = 0.0f;
	pBossParts->bDisp = false;
	pBossParts->bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
	pBossParts->bBlinking = false;
	pBossParts->fMove = 0.0f;
	pBossParts->nTimeLine = -1;
	pBossParts->nScore = 0;
	pBossParts->nLife = 3;
	pBossParts->nCounterAnim = 0;
	pBossParts->nPatternAnim = 0;
	pBossParts->nCounterState = 0;
	pBossParts->nCounterAttack = rand() % 300 + 150;
	pBossParts->nUseCounter = 600;
}