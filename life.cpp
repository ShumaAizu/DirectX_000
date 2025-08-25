//=============================================================================
//
//	���C�t�\������ [life.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================
#include "life.h"
#include "player.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define LIFE_POSX		(0.0f)				// ���C�t�̍��WX
#define LIFE_POSY		(0.0f)				// ���C�t�̍��WY
#define LIFE_SIZEX		(100.0f)			// ���C�t�̃T�C�YX
#define LIFE_SIZEY		(100.0f)			// ���C�t�̃T�C�YY

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureLife = NULL;				// ���C�t�̃e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffLife = NULL;			// ���C�t�̒��_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posLife;									// ���C�t�̈ʒu

//=============================================================================
//	���C�t�̏���������
//=============================================================================
void InitLife(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntLife = 0;

	// ������
	g_posLife = D3DXVECTOR3(LIFE_POSX, LIFE_POSY, 0.0f);

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\Life000.png", &g_pTextureLife);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_LIFE,		// �ő吔����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffLife,
		NULL);

	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX), LIFE_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX) + LIFE_SIZEX, LIFE_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX), LIFE_POSY + LIFE_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(LIFE_POSX + (nCntLife * LIFE_SIZEX) + LIFE_SIZEX, LIFE_POSY + LIFE_SIZEY, 0.0f);

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
		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}

//=============================================================================
//	���C�t�̏I������
//=============================================================================
void UninitLife(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureLife != NULL)
	{
		g_pTextureLife->Release();
		g_pTextureLife = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffLife != NULL)
	{
		g_pVtxBuffLife->Release();
		g_pVtxBuffLife = NULL;
	}
}

//=============================================================================
//	���C�t�̕`�揈��
//=============================================================================
void DrawLife(void)
{
	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffLife, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureLife);

	for (int nCntLife = 0; nCntLife < MAX_LIFE; nCntLife++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntLife * 4, 2);
	}
}

//=============================================================================
//	���C�t�̍X�V����
//=============================================================================
void UpdateLife(void)
{
	int nCntNumber = 0;

	// �v���C���[�̏����擾
	Player* pPlayer = GetPlayer();

	// �e�N�X�`�����W�̐ݒ�
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffLife->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntNumber = 0; nCntNumber < MAX_LIFE; nCntNumber++)
	{
		if (pPlayer->nLife >= nCntNumber + 1)
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
		}
		else
		{
			// �e�N�X�`�����W�̐ݒ�
			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
			pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffLife->Unlock();
}