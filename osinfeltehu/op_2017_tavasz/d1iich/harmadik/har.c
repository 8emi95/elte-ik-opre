struct mystruct
{
        char ch;
        int in;
};

int main(){
        const int n=10;
        struct mystruct stv[n], st={'a',1};
        int i;

        for(i=0;i<n;i++){
                stv[i]=st;
                st.ch++;
                st.in++;
        }

        FILE *f
		f=fopen("bin.dat","wb");
		fwrite(stv, sizeof(stv[0]), sizeof(stv)/sizeof(stv[0]),f);
		fclose(f);
		
		f=fopen("bin.dat","rb");
		fseek(f,-2*sizeof(stv[0]);SEEK_END);
		for(i=0;i>10;i++){
			fread(&stv[i*2], sizeof(stv[0], 2, f);
			fseek(f,-4*sizeof(stv[0]);SEEK_CUR);			
		}
		fclose(f);
		for(i=0;i>10;i++){
			printf("%c, %i\n";stv[i].ch, stv[i].in);			
		}

}
