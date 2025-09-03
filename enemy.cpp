//=============================================================================
//
//	�G�̏��� [enemy.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "enemy.h"
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
#define MAX_WARD		(256)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_apTextureEnemy[NUM_ENEMY] = {};			// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEnemy = NULL;					// ���_�o�b�t�@�ւ̃|�C���^
Enemy g_aEnemy[MAX_ENEMY];										// �G�̏��
int g_nCounterEnemy;

//====================================
//	�G�̏���������
//====================================
void InitEnemy(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntEnemy = 0;

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
		g_aEnemy[nCntEnemy].move = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].bBlinking = false;
		g_aEnemy[nCntEnemy].nTimeLine = -1;
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
	}

	g_nCounterEnemy = 0;

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
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);

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

	LoadEnemy();
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
		if (g_aEnemy[nCntEnemy].bUse == true)
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
	static int nFrameCounter = 90;

	nFrameCounter++;

	Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	// �g�p����
	for (nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		if (g_aEnemy[nCntEnemy].bUse == true)
		{// �����G���g�p����Ă�����
			Player* pPlayer = GetPlayer();		// �v���C���[�̏��擾
			float fAngleE_P = atan2f(pPlayer->pos.x - g_aEnemy[nCntEnemy].pos.x,		// �v���C���[�ƓG�Ƃ̊p�x�Z�o
									 pPlayer->pos.y - g_aEnemy[nCntEnemy].pos.y);

			// ���W���X�V
			g_aEnemy[nCntEnemy].pos += g_aEnemy[nCntEnemy].move;
			if (nFrameCounter % 180 == 0)
			{
				g_aEnemy[nCntEnemy].move.x *= -1.0f;
			}

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY - pCameraPos->y, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX - pCameraPos->x, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY - pCameraPos->y, 0.0f);

			switch (g_aEnemy[nCntEnemy].state)
			{// ��ԃ`�F�b�N
				// �ʏ���
			case ENEMYSTATE_NORMAL:
				g_aEnemy[nCntEnemy].nCounterAttack--;
				if (g_aEnemy[nCntEnemy].nCounterAttack <= 0)
				{
					if (pPlayer->state != PLAYERSTATE_DEATH)
					{// �v���C���[���������Ă����
						// �e���˂ƃN�[���^�C���ݒ�
						SetEnemyBullet(g_aEnemy[nCntEnemy].pos, 5.0f, 150, BULLETTYPE_ENEMY,SHOTTYPE_AIM, fAngleE_P);
						g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;
					}
				}
				break;

				// �_���[�W���
			case ENEMYSTATE_DAMAGE:
				g_aEnemy[nCntEnemy].nCounterState--;		// ��Ԏ������Ԃ����炷
				if (g_aEnemy[nCntEnemy].nCounterState <= 0)
				{// ��Ԏ������Ԃ��Ȃ��Ȃ�����
					if (g_aEnemy[nCntEnemy].nLife <= 1)
					{// �̗͂�1�ȉ��Ȃ�댯��Ԃ�
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_WARNING;

						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

						g_aEnemy[nCntEnemy].nCounterAttack = 0;			// �����U��������
					}
					else
					{// �̗͂��\���Ȃ�m�[�}����Ԃ�
						g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
						// ���_�J���[�̐ݒ�
						pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
						pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					}
				}
				break;

			case ENEMYSTATE_WARNING:
				g_aEnemy[nCntEnemy].nCounterAttack--;
				if (g_aEnemy[nCntEnemy].nCounterAttack <= 0)
				{
					if (pPlayer->state != PLAYERSTATE_DEATH)
					{// �v���C���[���������Ă�����
						// �����_���Ȋp�x�𐶐�
						float fAngle = (float)((rand() % 200 + 1 - 100) * 0.01f);

						// ���������p�x�ɒe��ݒ�
						SetEnemyBullet(g_aEnemy[nCntEnemy].pos, 10.0f, 150, BULLETTYPE_ENEMY, SHOTTYPE_NORMAL, fAngle * D3DX_PI);
						g_aEnemy[nCntEnemy].nCounterAttack = 6;			// �N�[���^�C����݂���

						g_aEnemy[nCntEnemy].nCounterState--;		// ��Ԏ������Ԃ����炷
						if (g_aEnemy[nCntEnemy].nCounterState <= 0)
						{// �������Ԃ��Ȃ��Ȃ�����
							// ��Ԑ؂�ւ��ƍăZ�b�g
							g_aEnemy[nCntEnemy].bBlinking  = g_aEnemy[nCntEnemy].bBlinking ? false : true;
							g_aEnemy[nCntEnemy].nCounterState = 3;
						}

						// ���_���W�̐ݒ�
						pVtx[0].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) - ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) - ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[1].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) + ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) - ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[2].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) - ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) + ENEMY_SIZEY - pCameraPos->y, 0.0f);
						pVtx[3].pos = D3DXVECTOR3((g_aEnemy[nCntEnemy].pos.x + (float)(rand() % 3 - 5)) + ENEMY_SIZEX - pCameraPos->x, (g_aEnemy[nCntEnemy].pos.y + (float)(rand() % 3 - 5)) + ENEMY_SIZEY - pCameraPos->y, 0.0f);
					}
				}
				break;

			case ENEMYSTATE_WAIT:
				g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
			}

			// �e�N�X�`�����X�V
			g_aEnemy[nCntEnemy].nCounterAnim++;
			if ((g_aEnemy[nCntEnemy].nCounterAnim % 18) == 0)
			{
				g_aEnemy[nCntEnemy].nPatternAnim = (g_aEnemy[nCntEnemy].nPatternAnim + 1) % 2;
				g_aEnemy[nCntEnemy].nCounterAnim = 0;
			}

			if (g_aEnemy[nCntEnemy].state == ENEMYSTATE_DAMAGE)
			{
				// �e�N�X�`�����W�̐ݒ�
				//pVtx[0].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);
				//pVtx[1].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.5f);
				//pVtx[2].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 1.0f);
				//pVtx[3].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 1.0f);
			}
			else
			{
				// �e�N�X�`�����W�̐ݒ�
				//pVtx[0].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.0f);
				//pVtx[1].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.0f);
				//pVtx[2].tex = D3DXVECTOR2(0.5f * g_aEnemy[nCntEnemy].nPatternAnim, 0.5f);
				//pVtx[3].tex = D3DXVECTOR2(0.5f * (g_aEnemy[nCntEnemy].nPatternAnim + 1), 0.5f);
			}

			if (pPlayer->state == PLAYERSTATE_DEATH)
			{// �v���C���[�����S��ԂȂ�F��߂�
				// ���_�J���[�̐ݒ�
				pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
			}
		}
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
}
//=============================================================================
//	�G�̐ݒ�
//=============================================================================
void SetEnemy(D3DXVECTOR3 pos, int nType, int nLife, int nTimeLine)
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
			g_aEnemy[nCntEnemy].type = (ENEMYTYPE)nType;
			g_aEnemy[nCntEnemy].nLife = nLife;
			g_aEnemy[nCntEnemy].nTimeLine = nTimeLine;
			g_aEnemy[nCntEnemy].state = ENEMYSTATE_WAIT;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y - ENEMY_SIZEY, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x - ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEnemy[nCntEnemy].pos.x + ENEMY_SIZEX, g_aEnemy[nCntEnemy].pos.y + ENEMY_SIZEY, 0.0f);

			g_nCounterEnemy++;
			//g_aEnemy[nCntEnemy].bUse = true;		// �G���g�p����Ă����Ԃɂ���
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
		SetRainbowParticle(g_aEnemy[nCntEnemy].pos, 3000.0f, 20);
		AddScore(100);

		g_nCounterEnemy--;

		pVtx += (nCntEnemy * 4);

		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterAnim = 0;
		g_aEnemy[nCntEnemy].nPatternAnim = 0;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

		PlaySound(SOUND_LABEL_SE_EXPLOSION);
	}
	else
	{// �G�̗̑͂��c���Ă�����
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_DAMAGE;		// �_���[�W��Ԃɂ���
		g_aEnemy[nCntEnemy].nCounterState = 12;

		PlaySound(SOUND_LABEL_SE_HIT);

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
void ResetEnemy(void)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEnemy->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEnemy = 0; nCntEnemy < MAX_ENEMY; nCntEnemy++, pVtx += 4)
	{
		// ������
		g_aEnemy[nCntEnemy].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEnemy[nCntEnemy].bUse = false;
		g_aEnemy[nCntEnemy].nLife = 3;
		g_aEnemy[nCntEnemy].nCounterState = 0;
		g_aEnemy[nCntEnemy].state = ENEMYSTATE_NORMAL;
		g_aEnemy[nCntEnemy].nCounterAttack = rand() % 300 + 150;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);

	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEnemy->Unlock();
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
//	�G�̃��[�h����
//=============================================================================
void LoadEnemy(void)
{
	// �f�o�C�X�|�C���^��錾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	FILE* pFile;

	pFile = fopen("data\\wave.txt", "r");

	if (pFile != NULL)
	{// �J������
		//���[�J���ϐ��錾
		char aString[MAX_WARD];		// �������ǂݍ���
		char aStrRelease[3];		// �s�v�� = ��ǂݍ���

		// ���ꂼ��̒l��ǂݍ���
		D3DXVECTOR3 pos = {};
		int type = 0;
		int life = 0;
		int timeline = 0;
		int nData;

		while (true)
		{
			nData = fscanf(pFile, "%s", &aString[0]);

			if (strcmp(&aString[0], "SETENEMY") == 0)
			{// SETENEMY��ǂݎ����
				while (true)
				{
					nData = fscanf(pFile, "%s", &aString[0]);

					if (strcmp(&aString[0], "POS") == 0)
					{// POS��ǂݎ����
						fscanf(pFile, "%s", &aStrRelease[0]);
					
						fscanf(pFile, "%f", &pos.x);
						fscanf(pFile, "%f", &pos.y);
						fscanf(pFile, "%f", &pos.z);
					}

					if (strcmp(&aString[0], "TYPE") == 0)
					{// TYPE��ǂݎ����
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &type);

					}

					if (strcmp(&aString[0], "LIFE") == 0)
					{// LIFE��ǂݎ����
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &life);
					}

					if (strcmp(&aString[0], "TIMELINE") == 0)
					{// LIFE��ǂݎ����
						fscanf(pFile, "%s", &aStrRelease[0]);

						fscanf(pFile, "%d", &timeline);
					}

					if (strcmp(&aString[0], "ENDSET") == 0)
					{// ENDSET��ǂݎ����
						SetEnemy(pos, type, life, timeline);
						break;
					}
				}
			}

			

			if (strcmp(&aString[0], "END_SCRIPT") == 0)
			{
				break;
			}
		}
	}
}