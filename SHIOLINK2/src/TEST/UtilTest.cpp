// TEST.cpp : コンソール アプリケーションのエントリ ポイントを定義します。
//

#include "stdafx.h"
#include "util.h"

TEST(UtilTest, RandomNumber) {
	ResetRandomNumber();
	UINT a = GenRandomNumber();
	for(int i=0; i<2; i++){
		UINT b = GenRandomNumber();
		if(a!=b) return;
	}
	ADD_FAILURE();
}