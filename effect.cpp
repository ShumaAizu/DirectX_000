//=============================================================================
//
//	�G�t�F�N�g���� [effect.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "effect.h"
#include "player.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define MAX_EFFECT		(71000)			// �G�t�F�N�g�̍ő吔

// �G�t�F�N�g�\����
typedef struct
{
	D3DXVECTOR3 pos;		// �ʒu
	D3DXVECTOR3 move;		// �ړ���
	D3DXCOLOR col;			// �F
	EFFECTSTATE state;		// ���
	float fRadius;			// ���a
	float fRadiusDecrease;	// ���a�����炷���x
	float fAlphaDecrease;	// �A���t�@�l�����炷���x
	int nLife;				// ����
	bool bUse;				// �g�p��
}Effect;

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureEffect = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffEffect = NULL;		// ���_�o�b�t�@�ւ̃|�C���^
Effect g_aEffect[MAX_EFFECT];							// �G�t�F�N�g�̏��

//====================================
//	�G�t�F�N�g�̏���������
//====================================
void InitEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^
	int nCntEffect = 0;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\effect000.jpg",
		&g_pTextureEffect);

	// ������
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		g_aEffect[nCntEffect].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].col = D3DXCOLOR(0.0f, 0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aEffect[nCntEffect].state = EFFECTSTATE_NORMAL;
		g_aEffect[nCntEffect].fRadius = 0;
		g_aEffect[nCntEffect].fRadiusDecrease = 0.25f;
		g_aEffect[nCntEffect].fAlphaDecrease = 0.025f;
		g_aEffect[nCntEffect].nLife = 0;
		g_aEffect[nCntEffect].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
	}

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_EFFECT,		// �G�t�F�N�g�̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffEffect,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void * *)&pVtx, 0);

	// ���_���̐ݒ�
	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y - g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x + g_aEffect[nCntEffect].fRadius,
								  g_aEffect[nCntEffect].pos.y + g_aEffect[nCntEffect].fRadius, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect[nCntEffect].col;
		pVtx[1].col = g_aEffect[nCntEffect].col;
		pVtx[2].col = g_aEffect[nCntEffect].col;
		pVtx[3].col = g_aEffect[nCntEffect].col;

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//====================================
//	�G�t�F�N�g�̏I������
//====================================
void UninitEffect(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureEffect != NULL)
	{
		g_pTextureEffect->Release();
		g_pTextureEffect = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffEffect != NULL)
	{
		g_pVtxBuffEffect->Release();
		g_pVtxBuffEffect = NULL;
	}
}

//====================================
//	�G�t�F�N�g�̕`�揈��
//====================================
void DrawEffect(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffEffect, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureEffect);

	// ���u�����f�B���O�����Z�����ɐݒ�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_ONE);

	// �g�p����
	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == true)
		{// �e���g�p����Ă���
			// �|���S���̕`��
			pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntEffect * 4, 2);
		}
	}

	// ���u�����f�B���O�����ɖ߂�
	pDevice->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
}

//====================================
//	�G�t�F�N�g�̍X�V����
//====================================
void UpdateEffect(void)
{
	int nCntEffect = 0;

	D3DXVECTOR3* pCameraPos = GetCamera();

	Effect* pEffect = &g_aEffect[0];

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++, pEffect++)
	{
		if (pEffect->bUse == true)
		{// �G�t�F�N�g���g�p����Ă���

			switch (pEffect->state)
			{
			case EFFECTSTATE_NORMAL:
				pEffect->fRadius -= pEffect->fRadiusDecrease;	// �傫��������������
				break;

			case EFFECTSTATE_COLLECT:

				Player* pPlayer = GetPlayer();
				float fAngle = atan2f(pPlayer->pos.x - pEffect->pos.x, pPlayer->pos.y - pEffect->pos.y);

				pEffect->move.x = sinf(fAngle) * 20.0f;
				pEffect->move.y = cosf(fAngle) * 20.0f;

				if (pEffect->pos == pPlayer->pos)
				{
					pEffect->bUse = false;
				}

				break;
			}

			if (g_aEffect[nCntEffect].fRadius <= 0)
			{// �傫����0�ȉ��ɂȂ�����
				pEffect->bUse = false;
			}

			pEffect->col.a -= pEffect->fAlphaDecrease;		// �����ɂ��Ă���

			pEffect->pos += pEffect->move;

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(pEffect->pos.x - pCameraPos->x - pEffect->fRadius,
									  pEffect->pos.y - pCameraPos->y - pEffect->fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(pEffect->pos.x - pCameraPos->x + pEffect->fRadius,
									  pEffect->pos.y - pCameraPos->y - pEffect->fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(pEffect->pos.x - pCameraPos->x - pEffect->fRadius,
									  pEffect->pos.y - pCameraPos->y + pEffect->fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(pEffect->pos.x - pCameraPos->x + pEffect->fRadius,
									  pEffect->pos.y - pCameraPos->y + pEffect->fRadius, 0.0f);

			// ���_�J���[�̐ݒ�
			pVtx[0].col = pEffect->col;
			pVtx[1].col = pEffect->col;
			pVtx[2].col = pEffect->col;
			pVtx[3].col = pEffect->col;

			g_aEffect[nCntEffect].nLife--;				// ���������

			// �g�p����
			if (pEffect->pos.x - pCameraPos->x > SCREEN_WIDTH || pEffect->pos.x - pCameraPos->x < 0 ||
				pEffect->pos.y - pCameraPos->y > SCREEN_HEIGHT || pEffect->pos.y - pCameraPos->y < 0)
			{// �����e����ʊO�ɏo����
				pEffect->bUse = false;		// �e���g�p���Ă��Ȃ���Ԃɂ���
			}

			if (pEffect->nLife < 0)
			{//�����������s������
				pEffect->bUse = false;		// �G�t�F�N�g���g�p���Ă��Ȃ���Ԃɂ���
				pEffect->move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
			}
			
		}

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}

//====================================
//	�G�t�F�N�g�̐ݒ�
//====================================
void SetEffect(D3DXVECTOR3 pos, D3DXVECTOR3 move, D3DXCOLOR col, float fRadius, float fRadiusDecrease, float fAlphaDecrease, int nLife)
{
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	D3DXVECTOR3* pCameraPos = GetCamera();

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntEffect = 0; nCntEffect < MAX_EFFECT; nCntEffect++)
	{
		if (g_aEffect[nCntEffect].bUse == false)
		{// �G�t�F�N�g���g�p���Ă��Ȃ�
			g_aEffect[nCntEffect].pos = pos;					// �󂯎�����ʒu����
			g_aEffect[nCntEffect].move = move;					// �󂯎�����ړ��ʂ���
			g_aEffect[nCntEffect].col = col;					// �󂯎�����F����
			g_aEffect[nCntEffect].state = EFFECTSTATE_NORMAL;
			g_aEffect[nCntEffect].fRadius = fRadius;			// �󂯎�������a����
			g_aEffect[nCntEffect].fRadiusDecrease = fRadiusDecrease;
			g_aEffect[nCntEffect].fAlphaDecrease = fAlphaDecrease;
			g_aEffect[nCntEffect].nLife = nLife;				// �󂯎������������

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y - g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x - g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y + g_aEffect[nCntEffect].fRadius, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntEffect].pos.x - pCameraPos->x + g_aEffect[nCntEffect].fRadius,
				g_aEffect[nCntEffect].pos.y - pCameraPos->y + g_aEffect[nCntEffect].fRadius, 0.0f);

			g_aEffect[nCntEffect].bUse = true;			// �G�t�F�N�g���g�p����Ă����Ԃɂ���
			break;		// ������for���𔲂���
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();

}

//====================================
//	�W�܂�p�[�e�B�N���̐ݒ�
//====================================
void SetCollectParticle(D3DXVECTOR3 pos, D3DXCOLOR col)
{
	int nCount = 0;
	float fMove;

	VERTEX_2D* pVtx = NULL;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffEffect->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntParticle = 0; nCntParticle < MAX_EFFECT; nCntParticle++, pVtx += 4)
	{
		if (g_aEffect[nCntParticle].bUse == true)
		{
			continue;
		}

		// �����_���Ȉʒu���Z�b�g
		g_aEffect[nCntParticle].pos.x = pos.x + (float)(rand() % 250 - 100);
		g_aEffect[nCntParticle].pos.y = pos.y + (float)(rand() % 250 - 100);

		// �����_���Ȕ��a���Z�b�g
		g_aEffect[nCntParticle].fRadius = (float)(rand() % 25 + 11);

		// �����_���Ȏ������Z�b�g
		g_aEffect[nCntParticle].nLife = 50;

		// �����_���Ȉړ��ʂ��Z�b�g
		fMove = 25.0f;

		g_aEffect[nCntParticle].state = EFFECTSTATE_COLLECT;

		g_aEffect[nCntParticle].col = col;

		float fAngle = atan2f(pos.x - g_aEffect[nCntParticle].pos.x, pos.y - g_aEffect[nCntParticle].pos.y);

		g_aEffect[nCntParticle].move.x = sinf(fAngle) * fMove;
		g_aEffect[nCntParticle].move.y = cosf(fAngle) * fMove;

		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x - g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y - g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x + g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y - g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x - g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y + g_aEffect[nCntParticle].fRadius, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(g_aEffect[nCntParticle].pos.x + g_aEffect[nCntParticle].fRadius,
			g_aEffect[nCntParticle].pos.y + g_aEffect[nCntParticle].fRadius, 0.0f);

		// ���_�J���[�̐ݒ�
		pVtx[0].col = g_aEffect[nCntParticle].col;
		pVtx[1].col = g_aEffect[nCntParticle].col;
		pVtx[2].col = g_aEffect[nCntParticle].col;
		pVtx[3].col = g_aEffect[nCntParticle].col;

		g_aEffect[nCntParticle].bUse = true;			// �G�t�F�N�g���g�p����Ă����Ԃɂ���

		nCount++;
		if (nCount > 5)
		{
			break;
		}
	}
	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffEffect->Unlock();
}