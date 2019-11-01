//////////////////////////////////offset bulucu fonksiyonu/////////////////////////////////////
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>
#include<process.h>
#include<windows.h>
#define LINE_SIZE 80
int count;

//Deðiþkenin tanýmlanýrken türünü belirleyen fonksiyon
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
//Girilen satýrda istenilen karakterin adresini geri döndüren fonksiyon
int findletterindis(char* line, int i, char letter){
//'@' sembolü gelirse harf olmayýncaya kadar stringde ileri gidiyor			
	if(letter=='@')
		while(isalpha(line[i])!=0)
		i++;
// Girilen harfi görünceye kadar stringde ileri gidiyor
	else
	while(line[i]!= letter ) 
		i++;
	return i;
}
//Printf ve scanf fonksiyonlarýnda deðiþkenin türünü belirlemek için kullanýlan fonksiyon 
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
//Deðiþkenleri dosyaya yazdýran fonksiyon
void writevariables(FILE* yazma,char** variable){
	int i=0;
	//Bütün deðiþkenleri teker teker dosyaya formatlý yazdýrma
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
//'_' karakteri olduðunda deðiþken yada print_line yada Get_value yada not_Equal vardýr
	while(strstr(line,"_")!=NULL){
		memset(name,0,50);
//Eðer print_line varsa oradaki '_' sembolünü geçici olarak # iþaretine döndürüyor, devamýndaki deðiþkenleri bulabilmek için
		if(strstr(line,"_LIN")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#';	
//DEFINE fonksiyonu nerede tanýmlýyorsa orada tanýmlansýn diye define da tanýmlanan deðiþkenleri almýyor
		}else if(strstr(line,"DEFINE")!=NULL){
			return variables;
//Get value komutundaki '_' sembolü geçici olarak # iþaretine döndürüyor, devamýndaki deðiþkenleri bulabilmek için
		}else if(strstr(line,"_VAL")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#'; 
//Not_equal komutundaki '_' sembolü geçici olarak # iþaretine döndürüyor, devamýndaki deðiþkenleri bulabilmek için
		}else if(strstr(line,"NOT_EQUAL")!=NULL){
			i=findletterindis(line,i,'_');
			line[i]='#';
//Bunlarý hiçbiri deðilse deðiþken olduðunu anlar ve fonksiyon baþlatýr
		}else{
			control=false;
			i=0;
			j=0;
//_'den önceki kýsmý saðdan sola doðru deðiþken ismi olarak name'e atar ve strrev fonksiyonu ile tersine çevirip doðr deðiþken ismini bulur
			i=findletterindis(line,i,'_');
			line[i]='#';
			i--;
			while((isalpha(line[i])!=0))
				name[j++]=line[i--];
			strrev(name);
//Memset fonksiyonu girilen dizinin elemanlarýný sýfýrlýyor. Önceki döngülerden bazý kelimeler kaldýðý için
			for(k=0;k<count;k++){
				memset(word,0,50);
				j=0;
				j=findletterindis(variables[k],j,'@');
				j++;
				m=0;
//Bulunan name variable matrisinde var mý kontrol ediyor
				while(isalpha(variables[k][j]))
					word[m++]=variables[k][j++];
				if(strcmp(word,name)==0)
					control=true;
			}
//Variables matrisine yeni deðer eklenen kýsým
			if(!control){
				i=findletterindis(line,i,'#');
				i++;
				variables=findtype(line,i,variables);	
				j=0;
//findtype fonksiyonundan gelen kelimenin uzunluðunu bulmak için 
				while(variables[count][j]!='\0')
					j++;
				k=0;
				while(name[k]!='\0')
					variables[count][j++]=name[k++];
				i=findletterindis(line,i,'@');
//eðer '[' varsa dizi demektir. Bunu konrol ediyor ve variables matrisine yazdýrýyor(dizinin boyutu girilmemiþse 100)
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
//DEFINE gelirse deðiþken tanýmlamasý yapýyor
    else if(strstr(line,"DEFINE")!=NULL){
	    i=0;//line indisi		
		j=0;//code indisi
		k=0;
		i=findletterindis(line,i,'_');
			i++;
//Degðiþkenin türüne bakýp bunu yazdýrýyor
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
//Name deðiþkeninin içine deðiþken ismini ters atýp code'a düzelterek yazýyor
		while(isalpha(line[i]))
			name[k++]=line[i--];
		while(k>0)
			code[j++]=name[--k];
		i=findletterindis(line,i,'_');
		i++;
		i=findletterindis(line,i,'@');
//Deðiþken dizi mi kontrol ediyor ve diziyse doðru formatta yazdýrýyor
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
//IF yada WHILE geldiðinde
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
//if geldiyse if, while geldiyse while yazýp parantez açýyor
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
//Deðiþkenin ismini code'a  yazdýrýyor
		while(isalpha(line[i])!=0)
			name[k++]=line[i--];
		k--;
		while(k >-1)
			code[j++]=name[k--];
		k++;
//_ görene kadar ilerliyor ve 1 arttýrýyor
		i=findletterindis(line,i,'_');
		i++;
		i=findletterindis(line,i,'@');
//Boþluk olduðu sürece ileri gidiyor
		while(line[i]==' ')
			i++;
//Eðer dizi ise on göre formatlý yazdýrýyor
		if(line[i]=='['){
			while(line[i]!='_')
				code[j++]=line[i++];
			code[j++]=']';
			i=findletterindis(line,i,']');
			i++;
		}
		while(line[i]==' ')
			i++;
//EQUAl ise == yapýyor
		if(line[i]=='E'){
			code[j++]='=';
			code[j++]='=';
			i=i+5;
//NOT_EQUAl ise != yapýyor
		}else if(line[i]=='N'){
			code[j++]='!';
			code[j++]='=';
			i=i+9;
//< > iþaretlerinden biriyse onu yazdýrýyor
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
//PRINT_LINE geldiyse dosyaya printf atýyor
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
//Deðiþkenin tipini type'a atýyor
				type=types(line[i+1],line[i+2]);
				k=0;
//Tamamýný yazdýrmak için aþaðýdaki while döngüsü yapýyor
				while(type[k]!='\0' && k<2)
		        	code[j++]=type[k++];
				i++;
				i=findletterindis(line,i,'@');
//Yazdýrýlan eleman dizi mi kontrol ediyor
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
//Eðer dolar iþareti varsa deðiþken vardýr
			while(line[i]=='$'){
				i++;
				code[j++]=',';
				while(line[i]!='_')
					code[j++]=line[i++];
				i++;
				i=findletterindis(line,i,'@');
//yazdýrýlacak eleman dizi mi kontrol ediyor
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
//Harf for döngüsündeki harfi tutuyor
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
//type dizisine deðiþkenin tipini atýyor
		type=types(line[i+1],line[i+2]);
		k=0;
//type dizisini code'a yazdýrýyor
		while(type[k]!='\0' && k<2)
		    code[j++]=type[k++];
		code[j++]='\"';
		code[j++]=',';
		code[j++]='&';
		i=0;
//line'da dolar görene kadar tekrar ilerliyor.Çünkü i deðeri deðiþkenin adýný yazdýrcak
		while(line[i]!='$')
			i++;
		i++;
		while(line[i]!='_')
			code[j++]=line[i++];
		i++;
		while(isalpha(line[i]))
			i++;
//Dizi var mý kontrol ediyor
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
//Atama iþlemleri yapýyor
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
        //Deðer atamada direk sayý gelirse
        if(line[i]=='-' || isdigit(line[i])){				
        	if(line[i]=='-')
        		code[j++]=line[i++];
       		while(isdigit(line[i]))
        		code[j++]=line[i++];
        }else{
        	//Deðer atamada herhangi bi deðiþken gelirse
        	while(line[i]!='_')									
        		code[j++]=line[i++];
        	i++;
        	i=findletterindis(line,i,'@');
        	//Deðer atamada dizi gelirse
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
		//Deðer atamada direk sayý gelirse
			if(line[i]=='-' || isdigit(line[i])){				
        	if(line[i]=='-')
        		code[j++]=line[i++];
       			while(isdigit(line[i])!=0)
        			code[j++]=line[i++];
        	}else{
        	//Deðer atamada herhangi bi deðiþken gelirse
        		while(line[i]!='_')										
       		 		code[j++]=line[i++];
       			i++;
        		i=findletterindis(line,i,'@');
        	//Deðer atamada dizi gelirse
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
 	//önce deðiþkenleri yazdýrmasý için bütün dosyayý deðiþken var mý diye tarýyor
 	while(!feof(fp)){
 		memset(deneme,0,100);
 		fgets(deneme,100,fp);
 		variables=findvariables(deneme,variables);
	}
	fclose(fp);
	//her satýr için denemeyi converter'a yollayarak c koduna dönüþtürüyor
	fp=fopen("input.txt","r");
 	while(!feof(fp)){
 		memset(deneme,0,100);
 		fgets(deneme,100,fp);
 		code=converter(deneme);
		fprintf(yazma,"%s\n",code);
		printf("Satir: %s\n",code);
	//Begin ilk geldiðinde deðiþkenlerin ekrana yazdýrýlmasý gerektiði bildirimi gidiyor
 		if(strstr(deneme,"BEGIN")!=NULL){
			counter++;
			if(counter==1)
				writevariables(yazma,variables);
		 }
	 }
	 //system("gcc -o calis.exe output.c");

    return 0;
}
