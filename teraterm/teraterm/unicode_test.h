/*
 * テストのために追加
 *	teraterm.hなどに入れると依存ファイルが多いためビルドに時間がかかるため
 *	テスト用ヘッダに分離
 *	将来はなくなる
 *
 */
#pragma once

#define	UNICODE_INTERNAL_BUFF	1	// 1 = 内部バッファunicode化

// UNICODE_INTERNAL_BUFFの影響を受けない
#define UNICODE_API				1	// UNICODE APIを許可
#define	UNICODE_DEBUG			1	// デバグ用機能enable
#define	UNICODE_DEBUG_CARET_OFF	0	// カーソル点滅系を止めるデバグ用
