/* 他のファイルで作った関数がありますとCコンパイラに教える */

extern void io_hlt(void);

/* 関数宣言なのに、{}がなくていきなり;を書くと、
	他のファイルにあるからよろしくね、という意味になるのです。 */

void HariMain(void)
{

//fin:
	//io_hlt(); /* これでnaskfunc.nasの_io_hltが実行されます */
	//goto fin;
    while(1){
        io_hlt();
    }

}
