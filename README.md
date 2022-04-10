# libyakisoba.so: ECM/EMM デコーダライブラリ

OishiiSlurper氏のSlurper-toolを基にライブラリ化。  
BCASカードやPC/SCサービスを用いることなく、本ライブラリ単体・独自APIで、  
ECMのbodyを受け取り復号しスクランブルキーK\_sを返す。  
また設定によりEMMからワークキーK\_wを取り出すことも可能。

## ビルド

```
$ git clone https://github.com//libyakisoba
$ cd libyakisoba; autoreconf -i; mkdir build; cd build
$ ../configure #(必要に応じて --prefixや--sysconfdir, --libdirを設定)
$ make
$ (sudo make install)
```

### [注意]

デフォルトでは`--prefix=/usr/local`で構成されるので、
本ライブラリは他のシステムライブラリとは別に  
`/usr/local/lib[64]`, `/usr/local/include`, `/usr/local/etc`にインストールされる。  
Fedora等のシステムではデフォルトで`/usr/local/lib64`を`ldconfig`が検索**しない**ので、
```
$ echo /usr/local/lib64 > /etc/ld.so.conf.d/local64.conf
```
などの追加設定が必要となることに注意。  
これらの設定やインストールを行わずに、LD\_LIBRARY\_PATHを指定して直接利用することも可能。

## 使用方法

- クライアントプログラムのソースで `#include <yakisoba.h>`する。
- `bcas_decodeECM()` や `bcas_decodeEMM()` をコールする。(yakisoba.h を参照)
- `libyakisoba.so` をリンクする。 (`-lyakisoba`)
- 実行前にあらかじめキーを記述した設定ファイルを用意しておく。

## 設定ファイル

ワークキーの値を記した設定ファイルをロード時に読み込んで使用するので、
下記のいずれかの場所に用意する必要がある。
1. 環境変数BCAS\_KEYS\_FILE に指定したパス
1. `~/.bcas_keys`
1. `$(sysconfdir)/bcas_keys`
上記の順に探されて、最初に見つかったファイルが使用される。  
フォーマットは付属のbcas\_keysを参照のこと。

EMMのデコードを行う場合は、
カードID及びマスターキーK\_mの値も、上記設定ファイルに記述する必要がある。

