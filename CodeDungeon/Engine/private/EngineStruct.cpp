#include "EngineDefine.h"
#include "EngineStruct.h"
#include <filesystem>
#include "UGameInstance.h"

namespace Engine {

#pragma region DX12_SHADER_DEFINES

	D3D12_INPUT_ELEMENT_DESC VTXPOINT_DELCARTION::Element[4] = {
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 36, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXDEFAULT_DECLARATION::Element[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXNORMAL_DECLARATION::Element[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXCUBE_DECLARATION::Element[2] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};

	D3D12_INPUT_ELEMENT_DESC VTXNORMALCUBE_DECLARATION::Element[3] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXSKYBOX_DECLARATION::Element[1] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXMODEL_DECLARATION::Element[8] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 }
	};

	D3D12_INPUT_ELEMENT_DESC VTXANIMMODEL_DECLARATION::Element[10] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDINDEX", 0, DXGI_FORMAT_R32G32B32A32_UINT, 0, 76, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "BLENDWEIGHT", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 92, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

	};


	D3D12_INPUT_ELEMENT_DESC VTXMODELCOLOR_DECLARATION::Element[9] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 1, DXGI_FORMAT_R32G32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 2, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 3, DXGI_FORMAT_R32G32_FLOAT, 0, 48, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 4, DXGI_FORMAT_R32G32_FLOAT, 0, 56, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 64, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{"COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 76, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0}
	};

#pragma endregion DX12_SHADER_DEFINES

#pragma region DX12_SHADER_INSTANCE_DEFINES

	D3D12_INPUT_ELEMENT_DESC VTXINSTANCERECT_DECLARATION::Element[6] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },

		 { "WORLDMATRIX", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 0,
			 D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },

		{ "WORLDMATRIX", 1, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 16,
			D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },

		{ "WORLDMATRIX", 2, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 32,
			D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 },

		{ "WORLDMATRIX", 3, DXGI_FORMAT_R32G32B32A32_FLOAT, 1, 48,
			D3D12_INPUT_CLASSIFICATION_PER_INSTANCE_DATA, 1 }
	};

#pragma endregion DX12_SHADER_INSTANCE_DEFINES

#pragma region CUSTIMER 

	_bool tagCusTimer::IsOver(const _double& _dTimeDelta)
	{
		if (fTimer >= fStandardTime)
		{
			isPass = true;
			return true;
		}
		fTimer += (_float)_dTimeDelta;
		return false;
	}

	void tagCusTimer::ResetTimer()
	{
		fTimer = 0.f;
		isPass = false;
	}

#pragma endregion CUSTIMER 

#pragma region FILEGROUP

	void tagFileGroup::AddFileGroup(const _wstring& _wstrFileName, CSHPTRREF<tagFileGroup> _spFileGroup)
	{
		UnderFileGroupList.insert(std::pair<_wstring, SHPTR<tagFileGroup>>(_wstrFileName, _spFileGroup));
	}

	void tagFileGroup::AddFileData(const _wstring& _wstrFileName, CSHPTRREF<FILEDATA> _spData)
	{
		FileDataList.insert(std::pair<_wstring, SHPTR<FILEDATA>>(_wstrFileName, _spData));
	}

	void tagFileGroup::FileAllData(const _wstring& _wstrEct, VECTOR<SHPTR<FILEDATA>>& _lsFileData)
	{
		for (auto& iter : FileDataList)
		{
			std::filesystem::path p = iter.second->wstrfilePath;
			if (p.extension() == _wstrEct)
			{
				_lsFileData.push_back(iter.second);
			}
		}
	}

	SHPTR<FILEDATA> tagFileGroup::FindData(const _wstring& _wstrFileName)
	{
		const auto& iter = FileDataList.find(_wstrFileName);
		if (iter == FileDataList.end())
			return nullptr;

		return iter->second;
	}

	SHPTR<tagFileGroup> tagFileGroup::FindGroup(const _wstring& _wstrFileName)
	{
		const auto& iter = UnderFileGroupList.find(_wstrFileName);
		if (iter == UnderFileGroupList.end())
			return nullptr;

		return iter->second;
	}

	SHPTR<tagFileGroup> tagFileGroup::FindUnder(const _wstring& _wstrFileName)
	{
		const auto& iter = UnderFileGroupList.find(_wstrFileName);
		if (iter == UnderFileGroupList.end()) {
			for (auto& iter : UnderFileGroupList) {
				FindUnder(_wstrFileName);
			}
		}
		return iter->second;
	}

	SHPTR<tagFileGroup> tagFileGroup::FindUpper(const _wstring& _wstrFileName)
	{
		SHPTR<tagFileGroup> spFileGroup = nullptr;
		if (nullptr != spParentsUpper)
		{
			if (spParentsUpper->wstrFolderName == _wstrFileName)
			{
				spFileGroup = spParentsUpper;
			}
			else
			{
				if (nullptr != spParentsUpper)
					spParentsUpper->FindUpper(_wstrFileName);
			}
		}
		return spFileGroup;
	}

#pragma endregion FILEGROUP

#pragma region ANIMSECTION

	void tagAnimFastSection::Convert(_double& _dValue, const _double& _dTimeAcc)
	{
		if (fStartSpot <= _dTimeAcc && _dTimeAcc <= fEndSpot)
			_dValue *= fFastValue;
	}

	_bool tagAnimFastSection::IsPass(const _double& _dTimeValue)
	{
		if (fStartSpot <= _dTimeValue && _dTimeValue <= fEndSpot)
			return true;

		return false;
	}

	_bool tagAnimClipSection::IsPass(const _wstring& _wstrName, const _double& _dTimeValue, const _double& _dSupTime)
	{
		if (_wstrName == wstrName)
		{
			if (fEndValue - _dSupTime <= _dTimeValue)
				return true;
		}
		return false;
	}

	_bool tagAnimClipSection::IsBetween(const _wstring& _wstrName, const _double& _dTimeValue)
	{
		if (_wstrName == wstrName)
		{
			if (fChange <= _dTimeValue && _dTimeValue <= fEndValue)
				return true;
		}
		return false;
	}

#pragma endregion ANIMSECTION 

#pragma region ANIMEVENTTYPE

	void ANIMEVENTDESC::RegisterEventFunc()
	{
		switch (MkEventType)
		{
		case MK_KEYBOARD:
		{
			switch (KeyPressType)
			{
			case KEYPRESSTYPE::KEY_UP:
				MkEventFunc = IsKeyboardUp;
				break;
			case KEYPRESSTYPE::KEY_DOWN:
				MkEventFunc = IsKeyboardDown;
				break;
			case KEYPRESSTYPE::KEY_PRESSING:
				MkEventFunc = IsKeyboardPressing;
				break;
			default:
				MkEventFunc = IsEmptyFunc;
				break;
			}
		}
			break;
		case MK_MOUSE:
		{
			switch (KeyPressType)
			{
			case KEYPRESSTYPE::KEY_UP:
			{
				switch (MouseButtonType)
				{
				case DIMOUSEBUTTON::DIMB_L:
					MkEventFunc = IsMouseLeftUp;
					break;
				case DIMOUSEBUTTON::DIMB_R:
					MkEventFunc = IsMouseRightUp;
					break;
				default:
					MkEventFunc = IsEmptyFunc;
					break;
				}
			}
				break;
			case KEYPRESSTYPE::KEY_DOWN:
			{
				switch (MouseButtonType)
				{
				case DIMOUSEBUTTON::DIMB_L:
					MkEventFunc = IsMouseLeftDown;
					break;
				case DIMOUSEBUTTON::DIMB_R:
					MkEventFunc = IsMouseRightDown;
					break;
				default:
					MkEventFunc = IsEmptyFunc;
					break;
				}
			}
			break;
			case KEYPRESSTYPE::KEY_PRESSING:
			{
				switch (MouseButtonType)
				{
				case DIMOUSEBUTTON::DIMB_L:
					MkEventFunc = IsMouseLeftPress;
					break;
				case DIMOUSEBUTTON::DIMB_R:
					MkEventFunc = IsMouseRightPress;
					break;
				default:
					MkEventFunc = IsEmptyFunc;
					break;
				}
			}
				break;		
			default:
				MkEventFunc = IsEmptyFunc;
				break;
			}
		}
			break;
		default:
			MkEventFunc = IsEmptyFunc;
			break;
		}
	}

	_char* ANIMEVENTDESC::SaveLoadPointer()
	{
		_char* Pointer = reinterpret_cast<_char*>(this) + sizeof(MkEventFunc);
		return Pointer;
	}

	void ANIMEVENTDESC::Reset()
	{
		isEventActive = false; 
		ubInputKey = 145;
		MkEventType = MKEVENTTYPE::MK_END;
		KeyPressType = KEYPRESSTYPE::KEY_END;
		MouseButtonType = DIMOUSEBUTTON::DIMB_WHEEL;
		MkEventFunc = IsEmptyFunc;
	}

	_bool ANIMEVENTDESC::IsMouseLeftDown(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_L);
	}

	_bool ANIMEVENTDESC::IsMouseRightDown(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnDown(DIMOUSEBUTTON::DIMB_R);
	}

	_bool ANIMEVENTDESC::IsMouseLeftUp(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnUp(DIMOUSEBUTTON::DIMB_L);
	}

	_bool ANIMEVENTDESC::IsMouseRightUp(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnUp(DIMOUSEBUTTON::DIMB_R);
	}

	_bool ANIMEVENTDESC::IsMouseLeftPress(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_L);
	}

	_bool ANIMEVENTDESC::IsMouseRightPress(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIMBtnPressing(DIMOUSEBUTTON::DIMB_R);
	}

	_bool ANIMEVENTDESC::IsKeyboardUp(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIKeyUp(_ubInputKey);
	}

	_bool ANIMEVENTDESC::IsKeyboardDown(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIKeyDown(_ubInputKey);
	}

	_bool ANIMEVENTDESC::IsKeyboardPressing(_ubyte _ubInputKey)
	{
		SHPTR<UGameInstance> spGameInstance = GET_INSTANCE(UGameInstance);
		return spGameInstance->GetDIKeyPressing(_ubInputKey);
	}

#pragma endregion ANIMEVENTTYPE

	_bool ANIMEVENTSECTIONDESC::IsAnimEventActive(const _double& _dTimeAcc)
	{
		isEventActive = false;
		if (_dTimeAcc >= dStartTime && _dTimeAcc <= dEndTime)
			isEventActive = true;

		return isEventActive;
	}

	bool ANIMOCURRESDESC::IsAnimOcurrs(const _double& _dTimeAcc) {
		isEventActive = false;

		if (_dTimeAcc <= dAnimOccursTime)
		{
			isEventActive = true;
			return true;
		}

		return false;
	}
}