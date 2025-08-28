//=============================================================================
//
//	�������ԏ��� [time.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "time.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define NUM_PLACE		(3)			// �������Ԃ̌���
#define TIME_POSX		(565)		// �������Ԃ̍��WX
#define TIME_POSY		(0)			// �������Ԃ̍��WY
#define TIME_SIZEX		(50)		// �������Ԃ̃T�C�YX
#define TIME_SIZEY		(75)		// �������Ԃ̃T�C�YY
#define LIMIT_TIME		(30)		// ��������

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureTime = NULL;				// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffTime = NULL;			// ���_�o�b�t�@�ւ̃|�C���^
D3DXVECTOR3 g_posTime;									// �������Ԃ̈ʒu
int g_nTime;											// �������Ԃ̒l

//=============================================================================
//	�������Ԃ̏���������
//=============================================================================
void InitTime(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntTime = 0;

	// ������
	g_posTime = D3DXVECTOR3(TIME_POSX, TIME_POSY, 0.0f);
	g_nTime = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,"data\\TEXTURE\\number000.png", &g_pTextureTime);


	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * NUM_PLACE,		// ���̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffTime,
		NULL);

	// ������
	g_nTime = LIMIT_TIME;

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntTime = 0; nCntTime < NUM_PLACE; nCntTime++)
	{
		// ���_���W�̐ݒ�
		pVtx[0].pos = D3DXVECTOR3(TIME_POSX + (nCntTime * TIME_SIZEX), TIME_POSY, 0.0f);
		pVtx[1].pos = D3DXVECTOR3(TIME_POSX + (nCntTime * TIME_SIZEX) + TIME_SIZEX, TIME_POSY, 0.0f);
		pVtx[2].pos = D3DXVECTOR3(TIME_POSX + (nCntTime * TIME_SIZEX), TIME_POSY + TIME_SIZEY, 0.0f);
		pVtx[3].pos = D3DXVECTOR3(TIME_POSX + (nCntTime * TIME_SIZEX) + TIME_SIZEX, TIME_POSY + TIME_SIZEY, 0.0f);

		// rhw�̐ݒ�
		pVtx[0].rhw = 1.0f;
		pVtx[1].rhw = 1.0f;
		pVtx[2].rhw = 1.0f;
		pVtx[3].rhw = 1.0f;

		// ���_�J���[�̐ݒ�
		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);
		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 0.0f, 1.0f);

		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f, 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f, 1.0f);

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//====================================
//	�������Ԃ̏I������
//====================================
void UninitTime(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureTime != NULL)
	{
		g_pTextureTime->Release();
		g_pTextureTime = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffTime != NULL)
	{
		g_pVtxBuffTime->Release();
		g_pVtxBuffTime = NULL;
	}
}

//====================================
//	�������Ԃ̕`�揈��
//====================================
void DrawTime(void)
{
	LPDIRECT3DDEVICE9 pDevice;				// �f�o�C�X�ւ̃|�C���^
	int nCntNumber;

	// �f�o�C�X�̎擾
	pDevice = GetDevice();

	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
	pDevice->SetStreamSource(0, g_pVtxBuffTime, 0, sizeof(VERTEX_2D));

	// ���_�t�H�[�}�b�g�̐ݒ�
	pDevice->SetFVF(FVF_VERTEX_2D);

	// �e�N�X�`���̐ݒ�
	pDevice->SetTexture(0, g_pTextureTime);

	for (nCntNumber = 0; nCntNumber < NUM_PLACE; nCntNumber++)
	{
		// �|���S���̕`��
		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntNumber * 4, 2);
	}
}

//====================================
//	�������Ԃ̍X�V����
//====================================
void UpdateTime(void)
{
	static int nTimeCounter = 0;
	int aTexU[NUM_PLACE];		// �e���̐������i�[
	int nCntNumber = 0;

	nTimeCounter++;
	if (nTimeCounter % 60 == 0)
	{
		g_nTime--;
		nTimeCounter = 0;
	}

	if (g_nTime <= 0)
	{
		g_nTime = 0;
	}

	aTexU[0] = g_nTime % 1000 / 100;
	aTexU[1] = g_nTime % 100 / 10;
	aTexU[2] = g_nTime % 10 / 1;

	// �e�N�X�`�����W�̐ݒ�
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffTime->Lock(0, 0, (void**)&pVtx, 0);

	for (nCntNumber = 0; nCntNumber < NUM_PLACE; nCntNumber++)
	{
		// �e�N�X�`�����W�̐ݒ�
		pVtx[0].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 0.0f);
		pVtx[1].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 0.0f);
		pVtx[2].tex = D3DXVECTOR2(0.1f * aTexU[nCntNumber], 1.0f);
		pVtx[3].tex = D3DXVECTOR2(0.1f * (aTexU[nCntNumber] + 1), 1.0f);

		if (g_nTime <= 10)
		{
			// ���_�J���[�̐ݒ�
			pVtx[0].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[1].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[2].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
			pVtx[3].col = D3DXCOLOR(1.0f, 0.0f, 0.0f, 1.0f);
		}

		pVtx += 4;
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffTime->Unlock();
}

//====================================
//	�������Ԃ̊m�F����
//====================================
bool CheckTime(void)
{
	if (g_nTime <= 0)
	{
		return false;
	}

	return true;
}