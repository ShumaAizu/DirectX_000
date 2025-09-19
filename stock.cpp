////=============================================================================
////
////	�c�@�\������ [stock.cpp]
////	Author : SHUMA AIZU
//// 
////=============================================================================
//#include "stock.h"
//#include "player.h"
//
////*****************************************************************************
//// �}�N����`
////*****************************************************************************
//#define STOCK_POSX		(0.0f)				// �c�@�̍��WX
//#define STOCK_POSY		(100.0f)			// �c�@�̍��WY
//#define STOCK_SIZEX		(50.0f)				// �c�@�̃T�C�YX
//#define STOCK_SIZEY		(50.0f)				// �c�@�̃T�C�YY
//
////*****************************************************************************
//// �O���[�o���ϐ�
////*****************************************************************************
//LPDIRECT3DTEXTURE9 g_pTextureStock = NULL;				// �c�@�̃e�N�X�`���ւ̃|�C���^
//LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffStock = NULL;			// �c�@�̒��_�o�b�t�@�ւ̃|�C���^
//D3DXVECTOR3 g_posStock;									// �c�@�̈ʒu
//
////=============================================================================
////	���C�t�̏���������
////=============================================================================
//void InitStock(void)
//{
//	// �f�o�C�X�̎擾
//	LPDIRECT3DDEVICE9 pDevice = GetDevice();
//	int nCntStock = 0;
//
//	// ������
//	g_posStock = D3DXVECTOR3(STOCK_POSX, STOCK_POSY, 0.0f);
//
//	// �e�N�X�`���̓ǂݍ���
//	D3DXCreateTextureFromFile(pDevice, "data\\TEXTURE\\stock000.png", &g_pTextureStock);
//
//
//	// ���_�o�b�t�@�̐���
//	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_STOCK,		// �ő吔����
//		D3DUSAGE_WRITEONLY,
//		FVF_VERTEX_2D,
//		D3DPOOL_MANAGED,
//		&g_pVtxBuffStock,
//		NULL);
//
//	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^
//
//	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
//	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
//	{
//		// ���_���W�̐ݒ�
//		pVtx[0].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX), STOCK_POSY, 0.0f);
//		pVtx[1].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX) + STOCK_SIZEX, STOCK_POSY, 0.0f);
//		pVtx[2].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX), STOCK_POSY + STOCK_SIZEY, 0.0f);
//		pVtx[3].pos = D3DXVECTOR3(STOCK_POSX + (nCntStock * STOCK_SIZEX) + STOCK_SIZEX, STOCK_POSY + STOCK_SIZEY, 0.0f);
//
//		// rhw�̐ݒ�
//		pVtx[0].rhw = 1.0f;
//		pVtx[1].rhw = 1.0f;
//		pVtx[2].rhw = 1.0f;
//		pVtx[3].rhw = 1.0f;
//
//		// ���_�J���[�̐ݒ�
//		pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//		pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
//
//		// �e�N�X�`�����W�̐ݒ�
//		pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//		pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//		pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//		pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
//
//		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffStock->Unlock();
//}
//
////=============================================================================
////	���C�t�̏I������
////=============================================================================
//void UninitStock(void)
//{
//	// �e�N�X�`���̔j��
//	if (g_pTextureStock != NULL)
//	{
//		g_pTextureStock->Release();
//		g_pTextureStock = NULL;
//	}
//
//	// ���_�o�b�t�@�̔j��
//	if (g_pVtxBuffStock != NULL)
//	{
//		g_pVtxBuffStock->Release();
//		g_pVtxBuffStock = NULL;
//	}
//}
//
////=============================================================================
////	���C�t�̕`�揈��
////=============================================================================
//void DrawStock(void)
//{
//	LPDIRECT3DDEVICE9 pDevice;			// �f�o�C�X�ւ̃|�C���^
//
//	// �f�o�C�X�̎擾
//	pDevice = GetDevice();
//
//	// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
//	pDevice->SetStreamSource(0, g_pVtxBuffStock, 0, sizeof(VERTEX_2D));
//
//	// ���_�t�H�[�}�b�g�̐ݒ�
//	pDevice->SetFVF(FVF_VERTEX_2D);
//
//	// �e�N�X�`���̐ݒ�
//	pDevice->SetTexture(0, g_pTextureStock);
//
//	for (int nCntStock = 0; nCntStock < MAX_STOCK; nCntStock++)
//	{
//		// �|���S���̕`��
//		pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntStock * 4, 2);
//	}
//}
//
////=============================================================================
////	���C�t�̍X�V����
////=============================================================================
//void UpdateStock(void)
//{
//	int nCntNumber = 0;
//
//	// �v���C���[�̏����擾
//	Player* pPlayer = GetPlayer();
//
//	// �e�N�X�`�����W�̐ݒ�
//	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^
//
//	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
//	g_pVtxBuffStock->Lock(0, 0, (void**)&pVtx, 0);
//
//	for (nCntNumber = 0; nCntNumber < MAX_STOCK; nCntNumber++)
//	{
//		if (pPlayer->nStock >= nCntNumber + 1)
//		{
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.0f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.0f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(0.5f, 1.0f);
//		}
//		else
//		{
//			// �e�N�X�`�����W�̐ݒ�
//			pVtx[0].tex = D3DXVECTOR2(0.5f, 0.0f);
//			pVtx[1].tex = D3DXVECTOR2(1.0f, 0.0f);
//			pVtx[2].tex = D3DXVECTOR2(0.5f, 1.0f);
//			pVtx[3].tex = D3DXVECTOR2(1.0f, 1.0f);
//		}
//
//		pVtx += 4;
//	}
//
//	// ���_�o�b�t�@���A�����b�N����
//	g_pVtxBuffStock->Unlock();
//}