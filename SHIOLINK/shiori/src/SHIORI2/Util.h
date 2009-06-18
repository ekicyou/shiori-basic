#pragma once

namespace Util
{
	namespace File
	{
		// �w�肵��codepage�̃e�L�X�g�Ƃ��ăt�@�C����ǂݍ��݂܂�
		CString ReadAllText(const CString& path, const UINT codePage);
	}

	namespace Text
	{
		// �����񂩂�[""] ���������܂��B
		CString UnQuot(const CString& text);

		// �R�[�h�y�[�W���擾���܂��B
		UINT GetCP(const CString& text);
	}
}
