//////////////////////////////////offset bulucu fonksiyonu/////////////////////////////////////
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<process.h>
#include<windows.h>
#define LINE_SIZE 80
int count;

//De�i�kenin tan�mlan�rken t�r�n� belirleyen fonksiyon
char** findtype(char* line,int i, char** variables){												
	if(line[i]=='c' && line[i+1]=='p')																
	strcpy(variables[count],"char* ");
		else if(line[i]=='f' && line[i+1]=='p')											
		strcpy(variables[count],"float* ");
			else if(line[i]=='i' && line[i+1]=='p')
			strcpy(variables[count],"int* ");
				else if(line[i]=='l' && line[i+1]=='p')
				strcpy(variables[count],"long int* ");
					else if(line[i]=='d' && line[i+1]=='p')
					strcpy(variables[count],"double* ");
							else if(line[i]=='l' && line[i+1]=='d')
							strcpy(variables[count],"long double ");
								else if(line[i]=='c')
								strcpy(variables[count],"char ");
									else if(line[i]=='f')
									strcpy(variables[count],"float ");
										else if(line[i]=='i')
										strcpy(variables[count],"int ");
											else if(line[i]=='l')
											strcpy(variables[count],"long ");
												else if(line[i]=='d')
												strcpy(variables[count],"double ");
													else if(line[i]=='f' && line[i+1]=='i')
													strcpy(variables[count],"file* ");
	return variables;
}
//Girilen sat�rda istenilen karakterin adresini geri d�nd�ren fonksiyon
int findletterindis(char* line, int i, char letter){
//'@' sembol� gelirse harf olmay�ncaya kadar stringde ileri gidiyor			
	if(letter=='@')
		while(isalpha(line[i])!=0)
		i++;
// Girilen harfi g�r�nceye kadar stringde ileri gidiyor
	else
	while(line[i]!= letter ) 
		i++;
	return i;
}
//Printf ve scanf fonksiyonlar�nda de�i�kenin t�r�n� belirlemek i�in kullan�lan fonksiyon 
char* types(char letter1, char letter2){
	if(letter1=='c')
	return "c";
		else if(letter1=='f')
		return "f";
			else if(letter1=='i')
			return "d";
				else if(letter1=='l')
				return "ld";
					else if(letter1=='d')
					return "f";
						else if(letter1=='l' && letter2=='d')
						return "lf";
	return "d";
}
//De�i�kenleri dosyaya yazd�ran fonksiyon
void writevariables(FILE* yazma,char** variable){
	int i=0;
	//B�t�n de�i�kenleri teker teker dosyaya formatl� yazd�rma
	while(i<count){
		if(variable[i][0]!='\0')
			fprintf(yazma,"%s\n",variable[i]);
		printf("Satir: %s\n",variable[i]);
		i++;
	}
}

char** findvariables(char* line,char** variables){
	int i=0,j,k,m,yer;
	bool control;
	char name[50],word[50];
//'_' karakteri oldu�unda de�i�ken yada print_line yada Get_value yada not_Equal vard�r
	while(strstr(line,"_")!=NULL){
		memset(name,0,50);
//E�er print_line varsa oradaki '_' sembol�n� ge�ici olarak # i�aretine d�nd�r�yor, devam�ndaki de�i�kenleri bulabilmek i�in
		if(strstr(line,"_LIN")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#';	
//DEFINE fonksiyonu nerede tan�ml�yorsa orada tan�mlans�n diye define da tan�mlanan de�i�kenleri alm�yor
		}else if(strstr(line,"DEFINE")!=NULL){
			return variables;
//Get value komutundaki '_' sembol� ge�ici olarak # i�aretine d�nd�r�yor, devam�ndaki de�i�kenleri bulabilmek i�in
		}else if(strstr(line,"_VAL")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#'; 
//Not_equal komutundaki '_' sembol� ge�ici olarak # i�aretine d�nd�r�yor, devam�ndaki de�i�kenleri bulabilmek i�in
		}else if(strstr(line,"NOT_EQUAL")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#';
//Bunlar� hi�biri de�ilse de�i�ken oldu�unu anlar ve fonksiyon ba�lat�r
		}else{
			control=false;
			i=0;
			j=0;
//_'den �nceki k�sm� sa�dan sola do�ru de�i�ken ismi olarak name'e atar ve strrev fonksiyonu ile tersine �evirip do�r de�i�ken ismini bulur
			i=findletterindis(line,i,'_');
			line[i]='#';
			i--;
			while((isalpha(line[i])!=0))
				name[j++]=line[i--];
			strrev(name);
//Memset fonksiyonu girilen dizinin elemanlar�n� s�f�rl�yor. �nceki d�ng�lerden baz� kelimeler kald��� i�in
			for(k=0;k<count;k++){
				memset(word,0,50);
				j=0;
				j=findletterindis(variables[k],j,'@');
				j++;
				m=0;
//Bulunan name variable matrisinde var m� kontrol ediyor
				while(isalpha(variables[k][j]))
					word[m++]=variables[k][j++];
				if(strcmp(word,name)==0)
					control=true;
			}
//Variables matrisine yeni de�er eklenen k�s�m
			if(!control){
				i=findletterindis(line,i,'#');
				i++;
				variables=findtype(line,i,variables);	
				j=0;
//findtype fonksiyonundan gelen kelimenin uzunlu�unu bulmak i�in 
				while(variables[count][j]!='\0')
					j++;
				k=0;
				while(name[k]!='\0')
					variables[count][j++]=name[k++];
				i=findletterindis(line,i,'@');
//e�er '[' varsa dizi demektir. Bunu konrol ediyor ve variables matrisine yazd�r�yor(dizinin boyutu girilmemi�se 100)
				if(line[i]=='['){
					i++;
					variables[count][j++]='[';
					variables[count][j++]='1';
					variables[count][j++]='0';
					variables[count][j++]='0';
					variables[count][j++]=']';
				}
				variables[count][j++]=';';
				count++;	
				}
			}
		}
		return variables;	
	}


char* converter(char *line){
    int i,j,k;
    char *code,harf,*type;
    char name[50];
	type = (char*)calloc(3,sizeof(char));
    code = (char*)calloc(50,sizeof(char));
    memset(code,0,50);
//Procedure gelirse int main()
    if(strstr(line,"PROCEDURE MAIN()")!=NULL)
        strcpy(code,"int main()");
//RETURN gelirse return 0
    else if(strstr(line,"RETURN")!=NULL)
    	strcpy(code,"return 0;");
//Begin gelirse '{'
    else if(strstr(line,"BEGIN")!=NULL){
    	strcpy(code,"{");
	}
//DEFINE gelirse de�i�ken tan�mlamas� yap�yor
    else if(strstr(line,"DEFINE")!=NULL){
	    i=0;//line indisi		
		j=0;//code indisi
		k=0;
		i=findletterindis(line,i,'_');
			i++;
//Deg�i�kenin t�r�ne bak�p bunu yazd�r�yor
		if(line[i]=='c' && line[i+1]=='p')
			strcpy(code,"char* ");
			else if(line[i]=='f' && line[i+1]=='p')
				strcpy(code,"float* ");
				else if(line[i]=='i' && line[i+1]=='p')
				strcpy(code,"int* ");
					else if(line[i]=='l' && line[i+1]=='p')
					strcpy(code,"long int* ");
						else if(line[i]=='d' && line[i+1]=='p')
						strcpy(code,"double* ");
							else if(line[i]=='l' && line[i+1]=='p')
							strcpy(code,"long double* ");
								else if(line[i]=='l' && line[i+1]=='d')
								strcpy(code,"long double ");
									else if(line[i]=='c')
									strcpy(code,"char ");
										else if(line[i]=='f')
										strcpy(code,"float ");
											else if(line[i]=='i')
											strcpy(code,"int ");
												else if(line[i]=='l')
												strcpy(code,"long ");
													else if(line[i]=='d')
													strcpy(code,"double ");
														else if(line[i]=='f' && line[i+1]=='i')
														strcpy(code,"file* ");
		j=findletterindis(code,j,'\0');
		i--;i--;
//Name de�i�keninin i�ine de�i�ken ismini ters at�p code'a d�zelterek yaz�yor
		while(isalpha(line[i]))
			name[k++]=line[i--];
		while(k>0)
			code[j++]=name[--k];
		i=findletterindis(line,i,'_');
		i++;
		i=findletterindis(line,i,'@');
//De�i�ken dizi mi kontrol ediyor ve diziyse do�ru formatta yazd�r�yor
		if(line[i]=='['){
			i++;
			code[j++]='[';
			while(isdigit(line[i]))
				code[j++]=line[i++];
			code[j++]=']';
			i++;
		}
		code[j++]=';';
	}
//END gelirse '}' 
    else if(strstr(line,"END")!=NULL)
        strcpy(code,"}");
//IF yada WHILE geldi�inde
    else if(strstr(line,"IF")!=NULL || strstr(line,"WHILE")!=NULL){
    	if(strstr(line,"IF")!=NULL){
    		strcpy(code,"if");
    	i=0;//line indisi
    	j=2;//code indisi
		}else if(strstr(line,"WHILE")!=NULL){
			strcpy(code,"while");
		i=0;
		j=5;	
		}
//if geldiyse if, while geldiyse while yaz�p parantez a��yor
    	do{
    		code[j++]='(';
    	i=findletterindis(line,i,'(');
    	if(line[i+1]=='(')
			code[j++]=line[++i];
		if(line[i+2]=='('){
			i++;
			code[j++]=line[++i];
		}
		i++;
		i=findletterindis(line,i,'_');
		i--;
		k=0;
//De�i�kenin ismini code'a  yazd�r�yor
		while(isalpha(line[i])!=0)
			name[k++]=line[i--];
		k--;
		while(k >-1)
			code[j++]=name[k--];
		k++;
//_ g�rene kadar ilerliyor ve 1 artt�r�yor
		i=findletterindis(line,i,'_');
		i++;
		i=findletterindis(line,i,'@');
//Bo�luk oldu�u s�rece ileri gidiyor
		while(line[i]==' ')
			i++;
//E�er dizi ise on g�re formatl� yazd�r�yor
		if(line[i]=='['){
			while(line[i]!='_')
				code[j++]=line[i++];
			code[j++]=']';
			i=findletterindis(line,i,']');
			i++;
		}
		while(line[i]==' ')
			i++;
//EQUAl ise == yap�yor
		if(line[i]=='E'){
			code[j++]='=';
			code[j++]='=';
			i=i+5;
//NOT_EQUAl ise != yap�yor
		}else if(line[i]=='N'){
			code[j++]='!';
			code[j++]='=';
			i=i+9;
//< > i�aretlerinden biriyse onu yazd�r�yor
		}else if(line[i]=='<' || line[i]=='>'){
			code[j++]=line[i++];
			if(line[i]=='=')
			 code[j++]=line[i++];
		}
		
		while(line[i]==' ')
			i++;
			if(isdigit(line[i])){
				while(isdigit(line[i]))
					code[j++]=line[i++];
			i=findletterindis(line,i,')');
			}else{
		while(line[i]!='_')
			code[j++]=line[i++];
		i++;
		i=findletterindis(line,i,'@');
		while(line[i]==' ')
			i++;
		if(line[i]=='['){
			while(line[i]!='_')
			code[j++]=line[i++];
			code[j++]=']';
			i=findletterindis(line,i,')');
		}}
			i++;
			i++;
			if(line[i]=='O'){
				code[j++]=')';
				code[j++]='|';
				code[j++]='|';
			}else if(line[i]=='A'){
				code[j++]=')';
				code[j++]='&';
				code[j++]='&';
			}
				i--;
				i--;
			memset(name,0,50);
		}while(line[i+2]=='O'||line[i+2]=='A');
			while(line[i]!='T' && line[i]!='D')
			code[j++]=line[i++];
	}
//PRINT_LINE geldiyse dosyaya printf at�yor
    else if(strstr(line,"PRINT_LINE")!=NULL){
        strcpy(code,"printf(\"");
        j=8;//code indisi
        i=0;//line indisi
    	i=findletterindis(line,i,'\"');
    	i++;
		while(line[i]!='\"'){
			if(line[i]=='$'){
				i++;
				i=findletterindis(line,i,'_');
				code[j++]='%';
//De�i�kenin tipini type'a at�yor
				type=types(line[i+1],line[i+2]);
				k=0;
//Tamam�n� yazd�rmak i�in a�a��daki while d�ng�s� yap�yor
				while(type[k]!='\0' && k<2)
		        	code[j++]=type[k++];
				i++;
				i=findletterindis(line,i,'@');
//Yazd�r�lan eleman dizi mi kontrol ediyor
				if(line[i]=='['){
					i=findletterindis(line,i,']');
					i++;
				}
				
			}
			
			code[j++]=line[i++];
		}
		code[j++]=' ';
		code[j++]='\\';
		code[j++]='n';
		code[j++]='/"';
		i=0;//line indisi
		while(line[i]!='\0'){
//E�er dolar i�areti varsa de�i�ken vard�r
			while(line[i]=='$'){
				i++;
				code[j++]=',';
				while(line[i]!='_')
					code[j++]=line[i++];
				i++;
				i=findletterindis(line,i,'@');
//yazd�r�lacak eleman dizi mi kontrol ediyor
				if(line[i]=='['){
					code[j++]='[';
				i++;
				while(line[i]!='_')
					code[j++]=line[i++];
				code[j++]=']';
				i=findletterindis(line,i,']');
				}
			}
		i++;
		}
		code[j++]=')';
		code[j++]=';';	
		code[j]='\0';
	}
	else if(strstr(line,"FOR")!=NULL && strstr(line,"LOOP")!=NULL){
		strcpy(code,"for(");
		i=0;//line indisi
		j=4;//code indisi
		i=findletterindis(line,i,'_');
//Harf for d�ng�s�ndeki harfi tutuyor
		harf=line[i-1];
		code[j++]=harf;
		code[j++]='='; 
		i=findletterindis(line,i,'.');	
		code[j++]=line[i-1];
		code[j++]=';';
		code[j++]=harf;
		code[j++]='<';
		code[j++]=line[i+2];
		code[j++]=';';
		code[j++]=harf;
		code[j++]='+';
		code[j++]='+';
		code[j++]=')';
	}
    
    else if(strstr(line,"GET_VAL")!=NULL){
		strcpy(code,"scanf(\"");
		j=7;//code indisi
		i=0;//line indisi
		i=findletterindis(line,i,'$');
		i=findletterindis(line,i,'_');
		code[j++]='%';
//type dizisine de�i�kenin tipini at�yor
		type=types(line[i+1],line[i+2]);
		k=0;
//type dizisini code'a yazd�r�yor
		while(type[k]!='\0' && k<2)
		    code[j++]=type[k++];
		code[j++]='\"';
		code[j++]=',';
		code[j++]='&';
		i=0;
//line'da dolar g�rene kadar tekrar ilerliyor.��nk� i de�eri de�i�kenin ad�n� yazd�rcak
		while(line[i]!='$')
			i++;
		i++;
		while(line[i]!='_')
			code[j++]=line[i++];
		i++;
		while(isalpha(line[i]))
			i++;
//Dizi var m� kontrol ediyor
		if(line[i]=='['){
			code[j++]='[';
			i++;
			while(line[i]!='_')
				code[j++]=line[i++];
			code[j++]=']';
		}
		code[j++]=')';
		code[j++]=';';
			                
    }
//Atama i�lemleri yap�yor
    else if(strstr(line,"<-")!=NULL){
    	i=0;
        j=0;
        while(line[i]!='_')
        code[j++]=line[i++];
        i++;
        i=findletterindis(line,i,'@');
//Dizi mi kontrol ediyor
        if(line[i]=='['){
        	code[j++]='[';
        	i++;
        	while(line[i]!='_')
        	code[j++]=line[i++];
        	code[j++]=']';
		}
        code[j++]='=';
        i=findletterindis(line,i,'<');
        i=i+2;
        while(line[i]==' ')
        	i++;
        //De�er atamada direk say� gelirse
        if(line[i]=='-' || isdigit(line[i])){				
        	if(line[i]=='-')
        		code[j++]=line[i++];
       		while(isdigit(line[i]))
        		code[j++]=line[i++];
        }else{
        	//De�er atamada herhangi bi de�i�ken gelirse
        	while(line[i]!='_')									
        		code[j++]=line[i++];
        	i++;
        	i=findletterindis(line,i,'@');
        	//De�er atamada dizi gelirse
        	if(line[i]=='['){									
        		code[j++]='[';
       		 	i++;
        		while(line[i]!='_')
        			code[j++]=line[i++];
        		code[j++]=']';
        		i++;
        	}
		}
		while(line[i]==' ')
			i++;
		if(line[i]== '+' || line[i]== '-' || line[i]== '*' || line[i]== '/' ){
			if(line[i]== '+')
				code[j++]='+';
				else if(line[i]== '-')
				code[j++]='-';
					else if(line[i]== '*')
					code[j++]='*';
						else if(line[i]== '/')
						code[j++]='/';
			i++;
			while(line[i]==' ')
				i++;
		//De�er atamada direk say� gelirse
			if(line[i]=='-' || isdigit(line[i])){				
        	if(line[i]=='-')
        		code[j++]=line[i++];
       			while(isdigit(line[i])!=0)
        			code[j++]=line[i++];
        	}else{
        	//De�er atamada herhangi bi de�i�ken gelirse
        		while(line[i]!='_')										
       		 		code[j++]=line[i++];
       			i++;
        		i=findletterindis(line,i,'@');
        	//De�er atamada dizi gelirse
        		if(line[i]=='['){									
        			code[j++]='[';
        			i++;
        			while(line[i]!='_')
        				code[j++]=line[i++];
        			code[j++]=']';
				}
			}
		}
		
		code[j++]=';';
		
	}
        
    return code;
}



int main(int argc, const char * argv[]) {
	int i,counter=0;
	count=0;
 	char* deneme=(char*)calloc(50,sizeof(char));
 	char* code=(char*)calloc(50,sizeof(char));
 	char** variables=(char**)calloc(50,sizeof(char*));
	for(i=0;i<50;i++)
		variables[i]=(char*)calloc(50,sizeof(char));
 	FILE *fp;
 	FILE *yazma;
 	fp=fopen("input.txt","r");
 	yazma=fopen("output.txt","w");
 	fprintf(yazma,"#include<stdlib.h>\n",code);
 	fprintf(yazma,"#include<stdio.h>\n",code);
 	//�nce de�i�kenleri yazd�rmas� i�in b�t�n dosyay� de�i�ken var m� diye tar�yor
 	while(!feof(fp)){
 		memset(deneme,0,100);
 		fgets(deneme,100,fp);
 		variables=findvariables(deneme,variables);
	}
	fclose(fp);
	//her sat�r i�in denemeyi converter'a yollayarak c koduna d�n��t�r�yor
	fp=fopen("input.txt","r");
 	while(!feof(fp)){
 		memset(deneme,0,100);
 		fgets(deneme,100,fp);
 		code=converter(deneme);
		fprintf(yazma,"%s\n",code);
		printf("Satir: %s\n",code);
	//Begin ilk geldi�inde de�i�kenlerin ekrana yazd�r�lmas� gerekti�i bildirimi gidiyor
 		if(strstr(deneme,"BEGIN")!=NULL){
			counter++;
			if(counter==1)
				writevariables(yazma,variables);
		 }
	 }
	 //system("gcc -o calis.exe output.c");

    return 0;
}
