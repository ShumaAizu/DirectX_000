//=============================================================================
//
//	�I�v�V�����̏��� [option.cpp]
//	Author : SHUMA AIZU
// 
//=============================================================================

#include "main.h"
#include "option.h"
#include "player.h"
#include "explosion.h"
#include "bullet.h"
#include "score.h"
#include "sound.h"
#include "input.h"
#include "camera.h"

//*****************************************************************************
// �}�N����`
//*****************************************************************************
#define OPTION_RADIUS	(32.0f)		// �I�v�V�����̔��a
#define OPTION_COOLTIME	(60)		// �I�v�V�����̃N�[���^�C��(ms)

//*****************************************************************************
// �O���[�o���ϐ�
//*****************************************************************************
LPDIRECT3DTEXTURE9 g_pTextureOption = NULL;						// �e�N�X�`���ւ̃|�C���^
LPDIRECT3DVERTEXBUFFER9 g_pVtxBuffOption = NULL;				// ���_�o�b�t�@�ւ̃|�C���^
Option g_aOption[MAX_OPTION] = {};								// �I�v�V�����̏��
D3DXVECTOR3 g_standard_rot = {};								// �W������
float g_fLengthOption = {};
float g_fAngleOption = {};

//====================================
//	�I�v�V�����̏���������
//====================================
void InitOption(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();
	int nCntOption = 0;

	// �e�N�X�`���̓ǂݍ���
	D3DXCreateTextureFromFile(pDevice,
		"data\\TEXTURE\\option000.png",
		&g_pTextureOption);

	// ������
	g_standard_rot = { 0.0f, 0.0f, 0.0f };

	g_fLengthOption = SQRTF(OPTION_RADIUS, OPTION_RADIUS);
	g_fAngleOption = atan2f(OPTION_RADIUS, OPTION_RADIUS);

	for (nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		g_aOption[nCntOption].pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aOption[nCntOption].move = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		g_aOption[nCntOption].fDistance = 0.0f;
		g_aOption[nCntOption].state = OPTIONSTATE_NORMAL;
		g_aOption[nCntOption].bUse = false;			// �g�p���Ă��Ȃ���Ԃɂ���
	}

	g_aOption[0].rot = D3DXVECTOR3(0.0f, 0.0f, -0.75f);
	g_aOption[1].rot = D3DXVECTOR3(0.0f, 0.0f, 0.75f);
	g_aOption[2].rot = D3DXVECTOR3(0.0f, 0.0f, -0.25f);
	g_aOption[3].rot = D3DXVECTOR3(0.0f, 0.0f, 0.25f);

	// ���_�o�b�t�@�̐���
	pDevice->CreateVertexBuffer(sizeof(VERTEX_2D) * 4 * MAX_OPTION,		// �I�v�V�����̐�����
		D3DUSAGE_WRITEONLY,
		FVF_VERTEX_2D,
		D3DPOOL_MANAGED,
		&g_pVtxBuffOption,
		NULL);

	VERTEX_2D *pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffOption->Lock(0, 0, (void * *)&pVtx, 0);

	for (nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		//// ���_���W�̐ݒ�
		//pVtx[0].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
		//pVtx[1].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
		//pVtx[2].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);
		//pVtx[3].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);

		pVtx[0].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z + D3DX_PI + g_fAngleOption) * g_fLengthOption;
		pVtx[0].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z + D3DX_PI + g_fAngleOption) * g_fLengthOption;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z - D3DX_PI - g_fAngleOption) * g_fLengthOption;
		pVtx[1].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z - D3DX_PI - g_fAngleOption) * g_fLengthOption;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z - g_fAngleOption) * g_fLengthOption;
		pVtx[2].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z - g_fAngleOption) * g_fLengthOption;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z + g_fAngleOption) * g_fLengthOption;
		pVtx[3].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z + g_fAngleOption) * g_fLengthOption;
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
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	�I�v�V�����̏I������
//=============================================================================
void UninitOption(void)
{
	// �e�N�X�`���̔j��
	if (g_pTextureOption != NULL)
	{
		g_pTextureOption->Release();
		g_pTextureOption = NULL;
	}

	// ���_�o�b�t�@�̔j��
	if (g_pVtxBuffOption != NULL)
	{
		g_pVtxBuffOption->Release();
		g_pVtxBuffOption = NULL;
	}
}

//=============================================================================
//	�I�v�V�����̕`�揈��
//=============================================================================
void DrawOption(void)
{
	// �f�o�C�X�̎擾
	LPDIRECT3DDEVICE9 pDevice = GetDevice();

	Player* pPlayer = GetPlayer();

	if (pPlayer->state != PLAYERSTATE_WAIT)
	{
		// ���_�o�b�t�@���f�[�^�X�g���[���ɐݒ�
		pDevice->SetStreamSource(0, g_pVtxBuffOption, 0, sizeof(VERTEX_2D));

		// ���_�t�H�[�}�b�g�̐ݒ�
		pDevice->SetFVF(FVF_VERTEX_2D);

		// �e�N�X�`���̐ݒ�
		pDevice->SetTexture(0, g_pTextureOption);

		for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
		{
			if (g_aOption[nCntOption].bUse == true)
			{// �I�v�V�������g�p����Ă���
				// �|���S���̕`��
				pDevice->DrawPrimitive(D3DPT_TRIANGLESTRIP, nCntOption * 4, 2);
			}
		}
	}
}

//=============================================================================
//	�I�v�V�����̍X�V����
//=============================================================================
void UpdateOption(void)
{
	Player* pPlayer = GetPlayer();				// �v���C���[�̏����擾

	Option* pOption = &g_aOption[0];			// �I�v�V�����̃|�C���^

	D3DXVECTOR3* pCameraPos = GetCamera();		// �J�����̏����擾

	VERTEX_2D* pVtx;							// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++, pOption++)
	{
		if (pOption->bUse == true)
		{
			switch (pOption->state)
			{
			case OPTIONSTATE_NORMAL:

				break;

			case OPTIONSTATE_COOLTIME:
				g_aOption[nCntOption].nCounterState--;
				if (g_aOption[nCntOption].nCounterState <= 0)
				{
					g_aOption[nCntOption].state = OPTIONSTATE_NORMAL;

					// ���_�J���[�̐ݒ�
					pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
					pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
				}

				break;
			}

			switch (g_aOption[nCntOption].substate)
			{
			case SUBOPTIONSTATE_BARRIER:
				break;
			}
		}

		if (GetKeyboardPress(DIK_Q) == true || GetJoypadPress(JOYKEY_LEFT_SHOULDER))
		{// ���L�[�������ꂽ
			// ���ɉ�]
			pOption->move.z += 0.000715f;
		}

		if (GetKeyboardPress(DIK_E) == true || GetJoypadPress(JOYKEY_RIGHT_SHOULDER))
		{// �E�L�[�������ꂽ
			// �E�ɉ�]
			pOption->move.z += -0.000715f;
		}

		g_standard_rot.z += pOption->move.z;

		pOption->move.z += (0.0f - pOption->move.z) * 0.1f;

		pOption->fAngle = atan2f(pOption->fDistance, pOption->fDistance);

		pOption->pos.x = pPlayer->pos.x + sinf((g_standard_rot.z + pOption->rot.z) * D3DX_PI) * pOption->fDistance;
		pOption->pos.y = pPlayer->pos.y + cosf((g_standard_rot.z + pOption->rot.z) * D3DX_PI) * pOption->fDistance;

		// ���_���W�̐ݒ�
		pVtx[0].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z + D3DX_PI + g_fAngleOption) * g_fLengthOption;
		pVtx[0].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z + D3DX_PI + g_fAngleOption) * g_fLengthOption;
		pVtx[0].pos.z = 0.0f;

		pVtx[1].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z - D3DX_PI - g_fAngleOption) * g_fLengthOption;
		pVtx[1].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z - D3DX_PI - g_fAngleOption) * g_fLengthOption;
		pVtx[1].pos.z = 0.0f;

		pVtx[2].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z - g_fAngleOption) * g_fLengthOption;
		pVtx[2].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z - g_fAngleOption) * g_fLengthOption;
		pVtx[2].pos.z = 0.0f;

		pVtx[3].pos.x = g_aOption[nCntOption].pos.x + sinf(g_aOption[nCntOption].rot.z + g_fAngleOption) * g_fLengthOption;
		pVtx[3].pos.y = g_aOption[nCntOption].pos.y + cosf(g_aOption[nCntOption].rot.z + g_fAngleOption) * g_fLengthOption;
		pVtx[3].pos.z = 0.0f;

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	�I�v�V�����̎擾
//=============================================================================
Option *GetOption(void)
{
	return &g_aOption[0];		// �I�v�V�����̏��̐擪�A�h���X��Ԃ�
}

//=============================================================================
//	�I�v�V�����̐ݒ�
//=============================================================================
void SetOption(D3DXVECTOR3 pos, float fDistance, float fAngle)
{
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	for (int nCntOption = 0; nCntOption < MAX_OPTION; nCntOption++)
	{
		if (g_aOption[nCntOption].bUse == false)
		{
			g_aOption[nCntOption].pos = pos;
			g_aOption[nCntOption].bUse = true;
			g_aOption[nCntOption].fDistance = fDistance;

			g_aOption[nCntOption].pos.x = sinf((fAngle) * g_aOption[nCntOption].fDistance);
			g_aOption[nCntOption].pos.y = cosf((fAngle) * g_aOption[nCntOption].fDistance);

			// ���_���W�̐ݒ�
			pVtx[0].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
			pVtx[1].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y - OPTION_RADIUS, 0.0f);
			pVtx[2].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x - OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);
			pVtx[3].pos = D3DXVECTOR3(g_aOption[nCntOption].pos.x + OPTION_RADIUS, g_aOption[nCntOption].pos.y + OPTION_RADIUS, 0.0f);

			break;
		}

		pVtx += 4;			// ���_�f�[�^�̃|�C���^��4���i�߂�
	}

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	�I�v�V�����̃q�b�g����
//=============================================================================
void HitOption(int nCntOption)
{
	g_aOption[nCntOption].state = OPTIONSTATE_COOLTIME;			// ��ԕύX
	g_aOption[nCntOption].nCounterState = OPTION_COOLTIME;		// �N�[���^�C���ݒ�

	// ���_���W�̍X�V
	VERTEX_2D* pVtx;			// ���_���ւ̃|�C���^

	// ���_�o�b�t�@�����b�N��,���_���ւ̃|�C���^���擾
	g_pVtxBuffOption->Lock(0, 0, (void**)&pVtx, 0);

	pVtx += (nCntOption * 4);

	// ���_�J���[�̐ݒ�
	pVtx[0].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[1].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[2].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);
	pVtx[3].col = D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.5f);

	// ���_�o�b�t�@���A�����b�N����
	g_pVtxBuffOption->Unlock();
}

//=============================================================================
//	�W�������擾����
//=============================================================================
D3DXVECTOR3* GetStandardRot(void)
{
	return &g_standard_rot;
}
