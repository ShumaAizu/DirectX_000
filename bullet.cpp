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

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	// �g�p����
	for (nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == true)
		{// �e���g�p����Ă���
			g_aBullet[nCntBullet].pos += g_aBullet[nCntBullet].move;

			CollisionOption(&g_aBullet[nCntBullet]);

			if (g_aBullet[nCntBullet].type == BULLETTYPE_PLAYER)
			{// �v���C���[�̒e
				// �G�t�F�N�g�̐ݒ�
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.75f, 0.1f, 0.1f, 1.0f), 25.0f, 50);
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f), 45.0f, 50);
				SetParticle(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.1f, 1.0f, 1.0f, 1.0f), 30.0f, 5);
				// �G�Ƃ̓����蔻��
				CollisionEnemy(&g_aBullet[nCntBullet]);
			}
			else if (g_aBullet[nCntBullet].type == BULLETTYPE_ENEMY)
			{// �G�̒e
				// �G�t�F�N�g�̐ݒ�
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.1f, 0.1f, 1.0f, 1.0f), 15.0f, 50);
				SetEffect(g_aBullet[nCntBullet].pos, D3DXVECTOR3(0.0f, 0.0f, 0.0f), D3DXCOLOR(0.75f, 0.1f, 0.1f, 1.0f), 30.0f, 50);
				// �v���C���[�Ƃ̓����蔻��
				CollisionPlayer(&g_aBullet[nCntBullet]);
			}

			// ���_���W�̐ݒ�
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

			// �g�p����
			if (g_aBullet[nCntBullet].pos.x > SCREEN_WIDTH || g_aBullet[nCntBullet].pos.x < 0 ||
				g_aBullet[nCntBullet].pos.y > SCREEN_HEIGHT || g_aBullet[nCntBullet].pos.y < 0)
			{// �����e����ʊO�ɏo����
				g_aBullet[nCntBullet].bUse = false;		// �e���g�p���Ă��Ȃ���Ԃɂ���
			}

			g_aBullet[nCntBullet].nLife--;				// ���������

			if (g_aBullet[nCntBullet].nLife < 0)
			{//�����������s������
				SetExplosion(g_aBullet[nCntBullet].pos, D3DXCOLOR(0.0f, 1.0f, 0.0f, 1.0f));
				g_aBullet[nCntBullet].bUse = false;		// �e���g�p���Ă��Ȃ���Ԃɂ���
			}

			if (g_aBullet[nCntBullet].bUse == false)
			{// �e���g�p���Ă��Ȃ���ԂɂȂ�����
				// ������
				g_aBullet[nCntBullet].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aBullet[nCntBullet].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aBullet[nCntBullet].rot = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
				g_aBullet[nCntBullet].nLife = 30;
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
void SetEnemyBullet(D3DXVECTOR3 pos, float fmove, int nLife, BULLETTYPE type, float fAngleE_P)
{
	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p���Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
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
void SetPlayerBullet(D3DXVECTOR3 pos, D3DXVECTOR3 move, int nLife, BULLETTYPE type)
{
	// �v���C���[���̎擾
	Player* pPlayer = GetPlayer();

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffBullet->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntBullet = 0; nCntBullet < MAX_BULLET; nCntBullet++)
	{
		if (g_aBullet[nCntBullet].bUse == false)
		{// �e���g�p���Ă��Ȃ�
			g_aBullet[nCntBullet].pos = pos;

			//���_���W�̐ݒ�
			pVtx[0].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + D3DX_PI + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[0].pos.z = 0.0f;

			pVtx[1].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - D3DX_PI - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[1].pos.z = 0.0f;

			pVtx[2].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z - g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[2].pos.z = 0.0f;

			pVtx[3].pos.x = g_aBullet[nCntBullet].pos.x + sinf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
			pVtx[3].pos.y = g_aBullet[nCntBullet].pos.y + cosf(g_aBullet[nCntBullet].rot.z + g_aBullet[nCntBullet].fAngle) * g_aBullet[nCntBullet].fLength;
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
				pOption->state == OPTIONSTATE_NORMAL)
			{// �����e���I�v�V�����ɂ������Ă�����
				// �I�v�V�����̃q�b�g����
				HitOption(nCntEnemy);
				pBullet->bUse = false;
			}
		}
	}
}