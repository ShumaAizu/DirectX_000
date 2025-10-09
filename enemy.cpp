//=============================================================================
//
//	�G�̏��� [enemy.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "game.h"
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
#define NUM_ENEMY		(5)			// �G�̎��
#define ENEMY_SCORE		(500)		// �G���j���̊l���X�R�A

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];										// �G�̏��
int g_nNumEnemy = {};

//====================================
//	�G�̏���������
//====================================
void InitEnemy(void)
{
	// �f�o�C�X�̎擾p
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy = 0;
	D3DXVECTOR3* pCameraPos = GetCamera();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy000.png",
		&g_apTextureEnemy[0]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy001.png",
		&g_apTextureEnemy[1]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy002.png",
		&g_apTextureEnemy[2]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy003.png",
		&g_apTextureEnemy[3]);

	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\enemy004.png",
		&g_apTextureEnemy[4]);

	// ������
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].fRadius = 0.0f;
		g_aEnemy[nCntEnemy].bDisp = false;
		g_aEnemy[nCntEnemy].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].bBlinking = false;
		g_aEnemy[nCntEnemy].fMove = 0.0f;
		g_aEnemy[nCntEnemy].nTimeLine = -1;
		g_aEnemy[nCntEnemy].nScore = 0;
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nCounterAttack = 60;
		g_aEnemy[nCntEnemy].nUseCounter = 600;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
	}

	g_nNumEnemy = 0;

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_ENEMY,		// �G�̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEnemy,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{

		float fAngle = atan2f(g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius);
		float fRadius = SQRTF(g_aEnemy[nCntEnemy].fRadius, g_aEnemy[nCntEnemy].fRadius);

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = g_aEnemy[nCntEnemy].pos.x - pCameraPos->x + sinf(fAngle) * fRadius;
		pVtx[0].pos.y = g_aEnemy[nCntEnemy].pos.y - pCameraPos->y + cosf(fAngle) * fRadius;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aEnemy[nCntEnemy].pos.x - pCameraPos->x + sinf(fAngle) * fRadius;
		pVtx[1].pos.y = g_aEnemy[nCntEnemy].pos.y - pCameraPos->y + cosf(fAngle) * fRadius;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aEnemy[nCntEnemy].pos.x - pCameraPos->x + sinf(D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.y = g_aEnemy[nCntEnemy].pos.y - pCameraPos->y + cosf(D3DX_PI - fAngle) * fRadius;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aEnemy[nCntEnemy].pos.x - pCameraPos->x + sinf(D3DX_PI + fAngle) * fRadius;
		pVtx[3].pos.y = g_aEnemy[nCntEnemy].pos.y - pCameraPos->y + cosf(D3DX_PI + fAngle) * fRadius;
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

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	�G�̏I������
//=============================================================================
void UninitEnemy(void)
{
	int nCntTex;
	// �e�N�X�`���̔j��
	for (nCntTex = 0; nCntTex < NUM_ENEMY; nCntTex++)
	{
		if (g_apTextureEnemy[nCntTex] != NULL)
		{
			// �e�N�X�`���̔j��
			g_apTextureEnemy[nCntTex]->Release();
			g_apTextureEnemy[nCntTex] = NULL;
		}
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEnemy != NULL)
	{
		g_pVtxBuffEnemy->Release();
		g_pVtxBuffEnemy = NULL;
	}
}

//=============================================================================
//	�G�̕`�揈��
//=============================================================================
void DrawEnemy(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEnemy, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true && g_aEnemy[nCntEnemy].bDisp == true)
		{// �G���g�p����Ă���
			// �e�N�X�`���̐ݒ�
			pDevice->SetTexture(0, g_apTextureEnemy[g_aEnemy[nCntEnemy].type]);

			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEnemy * 4, 2);
		}
	}
}

//=============================================================================
//	�G�̍X�V����
//=============================================================================
void UpdateEnemy(void)
{
	int nCntEnemy = 0;

	Enemy* pEnemy = &g_aEnemy[0];

	Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// �g�p����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{// �����G���g�p����Ă�����
			Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾
			float fAngleE_P = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x,		// �v���C���[�ƓG�Ƃ̊p�x�Z�o
									 pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

			// �X�R�A�����炷
			pEnemy->nScore--;
			if (pEnemy->type != ENEMYTYPE_BOSS && GetGameState() != GAMESTATE_TUTORIAL)
			{
				pEnemy->nUseCounter--;
			}

			float fAngle = atan2f(pEnemy->fRadius, pEnemy->fRadius);
			float fRadius = SQRTF(pEnemy->fRadius, pEnemy->fRadius);

			// ���_���W�̐ݒ�
			pVtx[0].pos.x = pEnemy->pos.x - pCameraPos->x + sinf(D3DX_PI + fAngle) * fRadius;
			pVtx[0].pos.y = pEnemy->pos.y - pCameraPos->y + cosf(D3DX_PI + fAngle) * fRadius;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = pEnemy->pos.x - pCameraPos->x + sinf(D3DX_PI - fAngle) * fRadius;
			pVtx[1].pos.y = pEnemy->pos.y - pCameraPos->y + cosf(D3DX_PI - fAngle) * fRadius;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = pEnemy->pos.x - pCameraPos->x + sinf(D3DX_PI + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.y = pEnemy->pos.y - pCameraPos->y + cosf(D3DX_PI + D3DX_PI - fAngle) * fRadius;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = pEnemy->pos.x - pCameraPos->x + sinf(D3DX_PI + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.y = pEnemy->pos.y - pCameraPos->y + cosf(D3DX_PI + D3DX_PI + fAngle) * fRadius;
			pVtx[3].pos.z = 0.0f;

			// ============== //
			// �G�̏��		  //
			// ============== //
			switch (pEnemy->state)
			{// ��ԃ`�F�b�N
				// �o�����
			case ENEMYSTATE_APPEAR:
				SetRainbowParticle(pEnemy->pos, 2500.0f, 10, D3DX_PI, -D3DX_PI);	// �p�[�e�B�N����ݒ�
				pEnemy->nCounterState--;			// �J�E���g�����炷
				if (pEnemy->nCounterState <= 0)
				{// �J�E���g��0�ȉ��ɂȂ����Ȃ�
					pEnemy->bDisp = true;				// �`��
					pEnemy->nCounterState = 0;			// �J�E���^�[���Z�b�g
					pEnemy->state = ENEMYSTATE_NORMAL;	// ��Ԃ�ʏ��
				}
				continue;
				break;

				// �ʏ���
			case ENEMYSTATE_NORMAL:

				break;

				// �_���[�W���
			case ENEMYSTATE_DAMAGE:
				pEnemy->nCounterState--;		// ��Ԏ������Ԃ����炷
				if (pEnemy->nCounterState <= 0)
				{// ��Ԏ������Ԃ��Ȃ��Ȃ�����
					pEnemy->state = ENEMYSTATE_NORMAL;
					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					
				}
				break;

			case ENEMYSTATE_WAIT:
				pEnemy->state = ENEMYSTATE_APPEAR;
				pEnemy->nCounterState = 60;
				break;
			}

			switch (pEnemy->type)
			{
			case ENEMYTYPE_FLAME:
				SetParticle(pEnemy->pos, D3DXCOLOR(1.0f, 0.2f, 0.2f, 1.0f), 2500.0f, ENEMYEFFECT_RADIUSDECREASE, ENEMYEFFECT_ALPHADECREASE, 5, D3DX_PI, -D3DX_PI);
				pEnemy->pos.x += sinf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;
				pEnemy->pos.y += cosf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;

				pEnemy->nCounterAttack--;
				if (pEnemy->nCounterAttack <= 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);
					float fAngleE_P = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
					pEnemy->nCounterAttack = 60;
					SetEnemyBullet(pEnemy->pos, 10.0f, 250, BULLETTYPE_FLAME, SHOTTYPE_AIM, fAngleE_P);
				}
				break;

			case ENEMYTYPE_WATER:
				SetParticle(pEnemy->pos, D3DXCOLOR(0.6f, 0.9f, 1.0f, 1.0f), 2500.0f, ENEMYEFFECT_RADIUSDECREASE, ENEMYEFFECT_ALPHADECREASE, 5, D3DX_PI, -D3DX_PI);
				if (pPlayer->state == PLAYERSTATE_NORMAL)
				{
					float fRotMove, fRotDest, fRotDiff;
					fRotMove = pEnemy->rot.z * D3DX_PI;
					fRotDest = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
					fRotDiff = fRotDest - fRotMove;

					if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += D3DX_PI * 2;
					}
					else if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= D3DX_PI * 2;
					}

					fRotMove += fRotDiff * 0.25f;

					if (fRotDiff < -D3DX_PI)
					{
						fRotDiff += D3DX_PI * 2;
					}
					else if (fRotDiff > D3DX_PI)
					{
						fRotDiff -= D3DX_PI * 2;
					}
					pEnemy->pos.x += sinf(fRotDest) * pEnemy->fMove;
					pEnemy->pos.y += cosf(fRotDest) * pEnemy->fMove;
				}
				break;

			case ENEMYTYPE_WIND:
				SetParticle(pEnemy->pos, D3DXCOLOR(0.8f, 1.0f, 0.7f, 1.0f), 2500.0f, ENEMYEFFECT_RADIUSDECREASE, ENEMYEFFECT_ALPHADECREASE, 5, D3DX_PI, -D3DX_PI);

				pEnemy->pos.x += sinf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;
				pEnemy->pos.y += cosf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;
				break;

			case ENEMYTYPE_EARTH:
				SetParticle(pEnemy->pos, D3DXCOLOR(0.5f, 0.25f, 0.0f, 1.0f), 2500.0f, ENEMYEFFECT_RADIUSDECREASE, ENEMYEFFECT_ALPHADECREASE, 5, D3DX_PI, -D3DX_PI);
				pEnemy->nCounterAttack--;
				if (pEnemy->nCounterAttack <= 0)
				{
					PlaySound(SOUND_LABEL_SE_SHOT001);
					float fAngleE_P = atan2f(pPlayer->pos.x - pEnemy->pos.x, pPlayer->pos.y - pEnemy->pos.y);
					pEnemy->nCounterAttack = 30;
					SetEnemyBullet(pEnemy->pos, 5.0f, 500, BULLETTYPE_EARTH, SHOTTYPE_AIM, fAngleE_P);
				}
				break;

			case ENEMYTYPE_BOSS:
				SetRainbowParticle(D3DXVECTOR3(pEnemy->pos.x - pEnemy->fRadius * 0.25f, pEnemy->pos.y - pEnemy->fRadius * 0.5f, 0.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);
				SetRainbowParticle(D3DXVECTOR3(pEnemy->pos.x + pEnemy->fRadius * 0.25f, pEnemy->pos.y - pEnemy->fRadius * 0.5f, 0.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);
				SetRainbowParticle(D3DXVECTOR3(pEnemy->pos.x - pEnemy->fRadius * 0.25f, pEnemy->pos.y + pEnemy->fRadius * 0.5f, 0.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);
				SetRainbowParticle(D3DXVECTOR3(pEnemy->pos.x + pEnemy->fRadius * 0.25f, pEnemy->pos.y + pEnemy->fRadius * 0.5f, 0.0f), 2500.0f, 5, D3DX_PI, -D3DX_PI);

	
				pEnemy->rot.z -= 0.0075f;

				pEnemy->pos.x += sinf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;
				pEnemy->pos.y += cosf(pEnemy->rot.z * D3DX_PI) * pEnemy->fMove;

				pEnemy->nCounterAttack--;
				if (pEnemy->nCounterAttack <= 0)
				{
					static bool bBulletChange = true;

					PlaySound(SOUND_LABEL_SE_SHOT001);
					float fAngleE_P;
					if (GetGameMode() == GAMEMODE_HARD)
					{
						if (bBulletChange == true)
						{
							fAngleE_P = 0.0f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 90;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, fAngleE_P * D3DX_PI);
							}
							fAngleE_P = 1.0f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P -= 0.2f)
							{
								pEnemy->nCounterAttack = 90;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, -fAngleE_P * D3DX_PI);
							}

							bBulletChange = bBulletChange ^ 1;
						}
						else
						{
							fAngleE_P = 0.1f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 90;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, fAngleE_P * D3DX_PI);
							}
							fAngleE_P = 0.1f;
							for (int nCntAngle = 1; nCntAngle < 6; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 90;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, -fAngleE_P * D3DX_PI);
							}
							bBulletChange = bBulletChange ^ 1;
						}
					}
					else
					{
						if (bBulletChange == true)
						{
							fAngleE_P = 0.0f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 180;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, fAngleE_P * D3DX_PI);
							}
							fAngleE_P = 1.0f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P -= 0.2f)
							{
								pEnemy->nCounterAttack = 180;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, -fAngleE_P * D3DX_PI);
							}

							bBulletChange = bBulletChange ^ 1;
						}
						else
						{
							fAngleE_P = 0.1f;
							for (int nCntAngle = 0; nCntAngle < 5; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 180;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, fAngleE_P * D3DX_PI);
							}
							fAngleE_P = 0.1f;
							for (int nCntAngle = 1; nCntAngle < 6; nCntAngle++, fAngleE_P += 0.2f)
							{
								pEnemy->nCounterAttack = 180;
								SetEnemyBullet(pEnemy->pos, 7.5f, 500, BULLETTYPE_BOSS, SHOTTYPE_NORMAL, -fAngleE_P * D3DX_PI);
							}
							bBulletChange = bBulletChange ^ 1;
						}
					}
				}
				break;

			}

			// �e�N�X�`�����X�V
			pEnemy->nCounterAnim++;
			if ((pEnemy->nCounterAnim % 18) == 0)
			{
				pEnemy->nPatternAnim = (pEnemy->nPatternAnim + 1) % 2;
				pEnemy->nCounterAnim = 0;
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
			if (pEnemy->pos.x <= 0 || pEnemy->pos.x >= WARLD_WIDTH ||
				pEnemy->pos.y <= 0 || pEnemy->pos.y >= WARLD_HEIGHT || 
				pEnemy->nUseCounter <= 0)
			{// ��ʊO�ɏo�� / �J�E���^�[�����؂���
				ResetEnemy(pEnemy);
				g_nNumEnemy--;
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
//=============================================================================
//	�G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, D3DXVECTOR3 rot, float fRadius, float fMove, int nType, int nLife, int nTimeLine, int nScore)
{
	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == false && g_aEnemy[nCntEnemy].state != ENEMYSTATE_WAIT)
		{// �G���g�p���Ă��Ȃ�
			g_aEnemy[nCntEnemy].pos = pos;
			g_aEnemy[nCntEnemy].rot = rot;
			g_aEnemy[nCntEnemy].type = (ENEMYTYPE)nType;
			g_aEnemy[nCntEnemy].fRadius = fRadius;
			g_aEnemy[nCntEnemy].fMove = fMove;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nTimeLine = nTimeLine;
			g_aEnemy[nCntEnemy].nScore = nScore;
			g_aEnemy[nCntEnemy].nCounterAttack = 60;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_WAIT;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - fRadius, g_aEnemy[nCntEnemy].pos.y - fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + fRadius, g_aEnemy[nCntEnemy].pos.y - fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - fRadius, g_aEnemy[nCntEnemy].pos.y + fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + fRadius, g_aEnemy[nCntEnemy].pos.y + fRadius, 0.0f);
			break;		// ������for���𔲂���
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	�G�̎擾
//=============================================================================
Enemy *GetEnemy(void)
{
	return &g_aEnemy[0];		// �G�̏��̐擪�A�h���X��Ԃ�
}

//=============================================================================
//	�G�̎擾
//=============================================================================
Enemy* GetBOSSEnemy(void)
{
	Enemy* pEnemy = &g_aEnemy[0];

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pEnemy++)
	{
		if (pEnemy->bUse == true)
		{
			if (pEnemy->type == ENEMYTYPE_BOSS)
			{
				return pEnemy;
			}
		}
	}
	return NULL;
}

//=============================================================================
//	�G�̃q�b�g����
//=============================================================================
void HitEnemy(int nCntEnemy, int nDamage)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	g_aEnemy[nCntEnemy].nLife -= nDamage;		// �G�̗̑͂����炷
	if (g_aEnemy[nCntEnemy].nLife <= 0)
	{// �G�̗̑͂��Ȃ��Ȃ�����
		//SetExplosion(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f));
		//SetParticle(g_aEnemy[nCntEnemy].pos, D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f), 50.0f, 10);
		SetRainbowParticle(g_aEnemy[nCntEnemy].pos, 3000.0f, 20, D3DX_PI, -D3DX_PI);
		AddScore(g_aEnemy[nCntEnemy].nScore);

		g_nNumEnemy--;

		pVtx += (nCntEnemy * 4);

		ResetEnemy(&g_aEnemy[nCntEnemy]);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		PlaySound(SOUND_LABEL_SE_EXPLOSION001);
	}
	else
	{// �G�̗̑͂��c���Ă�����
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		// �_���[�W��Ԃɂ���
		g_aEnemy[nCntEnemy].nCounterState = 12;

		PlaySound(SOUND_LABEL_SE_HIT001);

		pVtx += (nCntEnemy * 4);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);

	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}

//=============================================================================
//	�G�̃��Z�b�g����
//=============================================================================
void ResetEnemy(Enemy *pEnemy)
{
	if (pEnemy->type == ENEMYTYPE_BOSS)
	{
		BossParts* pBossParts = GetBossParts();
		for (int nCntBossParts = 0; nCntBossParts < MAX_BOSSPARTS; nCntBossParts++, pBossParts++)
		{
			if (pBossParts->bUse == false)
			{
				continue;
			}

			ResetBossParts(pBossParts);

		}
	}
	// ������
	pEnemy->pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	pEnemy->fRadius = 0.0f;
	pEnemy->bDisp = false;
	pEnemy->bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
	pEnemy->bBlinking = false;
	pEnemy->fMove = 0.0f;
	pEnemy->nTimeLine = -1;
	pEnemy->nScore = 0;
	pEnemy->nLife = 3;
	pEnemy->nCounterAnim = 0;
	pEnemy->nPatternAnim = 0;
	pEnemy->nCounterState = 0;
	pEnemy->nCounterAttack = rand() % 300 + 150;
	pEnemy->nUseCounter = 600;
	pEnemy->state = ENEMYSTATE_NORMAL;
}

//=============================================================================
//	�G�̏�������
//=============================================================================
void DeleteEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		g_aEnemy[nCntEnemy].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
	}
}

//=============================================================================
//	�G�̐����m�F����
//=============================================================================
bool CheckEnemy(void)
{
	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{
			return true;
		}
	}

	return false;
}

//=============================================================================
//	�G�̐��m�F����
//=============================================================================
int* GetNumEnemy(void)
{
	return &g_nNumEnemy;
}