#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


#define LINE_MAX_SIZE 500
#define TOKEN_MAX_SIZE 30
#define TOKEN_MAX_NUMBER 30

FILE * open_file(FILE *file_ptr);
int read_line(char * line);
int is_a_function(int line_length);
int is_a_condition(int line_length);
int is_a_loop(int line_length);
int is_a_relational(char x);
int is_operator(char x);
int is_a_function_call(int line_length);

int is_user_defined(char token[]);
int is_special_character(char token);
int check_openning_curly_bracket(FILE *file_ptr, char * line);
void is_a_definition();
void is_an_operation();
int is_in_user_defs(char token[]);
int is_def_type(char token[]);

char string_of_tokens[TOKEN_MAX_NUMBER][TOKEN_MAX_SIZE];

char  user_def[50][50] ;	
char spec_chars[]= {'{','}','[',']','(',')',
						  '.',':',';','?',',',
						  '\'','"','\\','#','*','-','/'};
char * def_dataType[5]= {"int","char","float","double","void"};

int block_opening_flag = 0;
int block_closing_flag = 0;

						  
int user_def_idex =0 ; 	
int def_flag=0;
int line_flag=0;
int int_flag=0;
int float_flag=0;
int char_flag=0;
int dot_flag=0; 
int def_step=0; 
int op_flag=0;
int opl_flag=0; 
int op_step=0;

int main(void)
{
	
	char line[LINE_MAX_SIZE];

    FILE *file_ptr;
	
	file_ptr =  open_file(file_ptr);
	int i = 0, j = 0, line_size;
	printf("\n Cheking the code syntax ....\n");

	while (fscanf(file_ptr, "%[^\n]\n", line) != EOF)
	{
		line_size = read_line(line); /* split the line into tokens*/
		memset(line, 0, sizeof line); /* clear the line varuable */
		
		/* if the opened == closed then we're outside all blocks and we need to search for a new block*/
		if(string_of_tokens[0][0] == '}')
		{
			block_closing_flag++;
		}	
		else if(block_opening_flag == block_closing_flag)
		{
			
			if(is_a_function(line_size)) /* if you found a function */
			{	if(is_in_user_defs(string_of_tokens[1]))
				{
					printf("function has multiple definitions\n");
					return 0;
				}
				else
				{
					strcpy(user_def[user_def_idex], string_of_tokens[1]);
					user_def_idex ++ ;
				}
				if(!check_openning_curly_bracket(file_ptr, line))
					return 0;
			}
			else
			{
				printf("code outside all blocks\n");
				return 0;
			}
		}
		else if(block_opening_flag > block_closing_flag)  /*I'm inside a block*/
		{

			line_flag=0;
			def_flag=0;
			def_step=0;
			op_flag=0;
			op_step=0;
			opl_flag=0;
			
			is_a_definition();
			is_an_operation();
			if(line_flag || opl_flag)
			{
				continue;
			}
			else if(is_a_loop(line_size)) /* if you found a function */
			{	
				if(!check_openning_curly_bracket(file_ptr, line))
					return 0;
			}
			else if(is_a_condition(line_size))
			{
				if(!check_openning_curly_bracket(file_ptr, line))
					return 0;
			}
			else if(is_a_function_call(line_size))
			{
					continue;
			}			
			else
			{
				printf("expresion not recognized\n");
				return 0;
			}
		}
		else  /* means block_opening_flag < block_closing_flag*/
		{
			printf("curly bracket closed before openning\n");
			return 0;
		}
	
	}

	if (block_opening_flag > block_closing_flag)
	{
			printf("missing closing curly bracket/s\n");
			
	} 
	else if (block_opening_flag < block_closing_flag)
	{
			printf("too many closing curly brackets\n");
	}
	else
	{
			printf(" No syntax errors found, code compiled successfully....\n");
	}
	return 0;
}


int is_a_function_call(int line_length)
{
	if(string_of_tokens[1][0] == '(' 
		&& string_of_tokens[line_length -2][0] == ')'
		&& string_of_tokens[line_length -1][0] == ';')
		
			if(is_in_user_defs(string_of_tokens[0]))
				return 1;
			else
				printf("undefined function\n");
	return 0;
}

int check_openning_curly_bracket(FILE *file_ptr,char * line)
{

	fscanf(file_ptr, "%[^\n]\n", line); /* read the next line to check if it's = '{'*/
	read_line(line); /* split the line into tokens*/
	memset(line, 0, sizeof line); /* clear the line varuable */
	if(string_of_tokens[0][0] != '{')
	{
		printf("error no opening curly bracket\n");
		return 0;
	}
	else
	{
		block_opening_flag++;
	}

	return 1;
}

FILE * open_file(FILE *file_ptr)
{
	if ((file_ptr = fopen("file.c", "r")) == NULL)
	{
        printf("Error! opening file");
        exit(1);
	}
	return file_ptr;
}


int read_line(char * line)
{
	int i = 0, j = 0,k = 0;

	while(line[j] != '\0')
	{
		while(line[j] != ' ' && line[j] != '\0')
		{
			string_of_tokens[k][i] = line[j];
			j++;
			i++;				
		}
		string_of_tokens[k][i] = '\0';
		j++;
		i = 0;
		k++;
	}

	return k;
}


int is_a_function(int line_length)
{
	int i, valid_dataType = 0;
	
	valid_dataType = is_def_type(string_of_tokens[0]);
	
	if(is_def_type)
	{
		if(is_user_defined(string_of_tokens[1]))
		{
			if(string_of_tokens[2][0] == '(')
			{

				if(string_of_tokens[line_length-1][0] == ')')
				{

					/* if(line_length-1 > 3) // contain params
					{
						i = 3;
						// check the first param
						if(is_def_type(string_of_tokens[i]) 
							&& is_user_defined(string_of_tokens[++i]))
						while(string_of_tokens[i][0] != ')')
						{
							if(string_of_tokens[i][0] == ',')
								if(is_def_type(string_of_tokens[i+1]) 
									&& is_user_defined(string_of_tokens[i+2]))
									
										return 1;
							
							i++;
						}
					}*/
					return 1;
				}
			}
		}
	}
	return 0;
}

int is_def_type(char token[])
{
	int i;
	for(i = 0; i < 5; i++)
	{
		if(strcmp(token,def_dataType[i]) == 0)
		{
			return 1;
		}
	}
	return 0;
}

int is_in_user_defs(char token[])
{
	int i;
	for(i = 0; i < 50; i++)
	{
		if(strcmp(user_def[i],token) == 0)
			return 1;
	}
	return 0;
}

int is_user_defined(char token[])
{
	int i;
	if(!(token[0] == '_' || isalpha(token[0])))
		return 0;
	
	for(i = 0; token[i] != '\0'; i++)
	{
		if(is_special_character(token[i]))
			return 0;
	}
	return 1;
	
}


int is_special_character(char token)
{
	int i;
	char spec_chars[]= {'{','}','[',']','(',')',
						  '.',':',';','?',',',
						  '\'','"','\\','#'};
	
	for(i = 0; i < sizeof(spec_chars); i++)
	{
		if(token == spec_chars[i])
			return 1;
	}
	return 0;	
}


int is_a_loop(int line_length)
{
	int i = 0;
	char iterator[TOKEN_MAX_SIZE];
	
	if(strcmp(string_of_tokens[0],"for") == 0)
	{
		if(string_of_tokens[1][0] == '(' && string_of_tokens[line_length-1][0] == ')')
		{		
			if(is_in_user_defs(string_of_tokens[2])
				&& string_of_tokens[3][0] == '=' 
				&& isdigit(string_of_tokens[4][0])
				&& string_of_tokens[5][0] == ';')
			{
				strcpy(iterator,string_of_tokens[2]);
				if(is_user_defined(string_of_tokens[6])
				&& is_a_relational(string_of_tokens[7][0])
				&& isdigit(string_of_tokens[8][0])
				&& string_of_tokens[9][0] == ';')
				{

					if(!strcmp(string_of_tokens[10],iterator)  && (!strcmp(string_of_tokens[11],"++") || !strcmp(string_of_tokens[11],"--"))
					|| !strcmp(string_of_tokens[11],iterator)  && (!strcmp(string_of_tokens[10],"++") || !strcmp(string_of_tokens[10],"--"))
					|| !strcmp(string_of_tokens[10],iterator) && is_operator(string_of_tokens[11][0]) && isdigit(string_of_tokens[11][0]))
					{
						return 1;
					}
				}
			}
		}
		else
			printf("error in for loop syntax");
	}
	
	return 0;
}

int is_a_relational(char x)
{
	if(x == '<' || x == '>')
		return 1;
	else
		return 0;
}

int is_operator(char x)
{
	if( x == '+' || x == '-' ||x == '/' ||x == '*' ||x == '=' )
		return 1;
	return 0;
}

int is_a_condition(int line_length)
{
	if(strcmp(string_of_tokens[0],"if") == 0)
	{	
		if(string_of_tokens[1][0] == '(')
		{
			//if(conditional_operators(string_of_tokens[5])) //if you have 2 conditions
			//{
				if(is_in_user_defs(string_of_tokens[2]))
				{
					if(is_a_relational(string_of_tokens[3][0]))
					{
						if(is_in_user_defs(string_of_tokens[4]) || isdigit(string_of_tokens[4][0]))
						{
							if(string_of_tokens[line_length-1][0] == ')')
							{
								return 1;
							}
						}
					}
				}

			//}
			
		}
		printf("error in the if statement\n");
	}

	return 0;
	
}


void is_a_definition ()
{ 
	if (def_step==0)
	{ 	
		def_flag=0;
		line_flag=0;
		for(int i= 0; i < 5; i++)
		{
			if(strcmp(string_of_tokens[def_step],def_dataType[i]) == 0)
			{

				if ((strcmp(string_of_tokens[def_step],"int") == 0))
				{
					int_flag=1;
					float_flag=0;
					char_flag=0;
					//printf(" is d") ;
				}
				else if ((strcmp(string_of_tokens[def_step],"char") == 0))
				{
					int_flag=0;
					float_flag=0;
					char_flag=1;
				}
				else if ((strcmp(string_of_tokens[def_step],"float") == 0))
				{
					int_flag=0;
					float_flag=1;
					char_flag=0;
				}
				def_step ++ ;
				def_flag=1;
				
				break ;
				
			}
			
			
		} 
		if (def_flag==0)
		{
			def_flag=0;
			def_step=0;
			//printf("*****") ;
			return  ;
			
		}
			
			
		
	}

	if 	(def_step==1)
	{
		
		if (isdigit(string_of_tokens[def_step][0]))
		{
			def_flag=0;
			def_step=0;
			//printf(" is d") ;
			
			return; 
		}
		
			
			
		for( int j= 0; j < strlen(string_of_tokens[def_step]); j++ )
	   {    
			for(int i= 0; i < strlen(spec_chars); i++)
			{
				if(string_of_tokens[def_step][j]==spec_chars[i]) 
				{  def_flag=0;
				   def_step=0;
				   return ; 
				}
		
			}  
		}
			
		def_flag=1;
		 
		//printf(" \nis d\n") ;
		int i=0;
		for ( i = 0 ; i< strlen(string_of_tokens[def_step]) ; i++)
		{ 
			user_def[user_def_idex][i]=string_of_tokens[def_step][i] ; 
		 
		// printf("%s\n",user_def);
		}
		//user_def[user_def_idex][++i]='\0';
		//puts(user_def[user_def_idex]) ;
		user_def_idex ++ ; 
		def_step ++ ;
	}
		
	if 	(def_step==2)
	{
		def_flag=0;
		line_flag=0;
		if (string_of_tokens[def_step][0] == ';')
		{ 
			def_flag=1;
			line_flag=1;
			def_step=0 ;
			// printf(" \nis d\n") ;
			return ;
		}
		else if (string_of_tokens[def_step][0] == '=')
		{
			def_flag=1;	 
			def_step++ ;
			 // printf(" \nis d\n") ;		
			  
		}
		else 
		{
			def_step=0;
			def_flag=0;
			return ; 
			
		}
	}
	
	if(def_step==3)
	{
		def_flag=0;
		if(int_flag==1&&char_flag==0&&float_flag==0)
		{
			
			for( int j= 0; j < strlen(string_of_tokens[def_step]); j++ )
			{
				if (isdigit(string_of_tokens[def_step][j]))
				{
					 
				}
				else 
				{
					def_step=0;
					def_flag=0;
					return ; 
				}
			}
			def_flag=1; 
			def_step++ ;		   
		}
		else if(int_flag==0&&char_flag==1&&float_flag==0)
		{
			if ( string_of_tokens[def_step][0]=='\'' &&(isalpha(string_of_tokens[def_step][1]) ||isdigit(string_of_tokens[def_step][1])) && string_of_tokens[def_step][2]=='\'' )
			{
				def_flag=1; 
				def_step++ ;
			}
			else 
			{
				def_step=0;
				def_flag=0;
				return ; 
			}
				
		} 
		else if(int_flag==0&&char_flag==0&&float_flag==1)
		{
			dot_flag=0;
			for( int j= 0; j < strlen(string_of_tokens[def_step]); j++ )
			{
				if ((isdigit(string_of_tokens[def_step][j])||string_of_tokens[def_step][j]=='.'))
				{
					if (string_of_tokens[def_step][j]=='.')
					{
						dot_flag ++ ; 
					}
					if(dot_flag>1)
					{
						def_step=0;
						def_flag=0;
						return ; 
					}
					
				}
				else 
				{
				
					def_step=0;
					def_flag=0;
					return ; 
				}
				
			}
			def_flag=1; 
			def_step++ ;
		}
			
			
	}
		
	if 	(def_step==4)
	{
		def_flag=0;
		line_flag=0;
		if (string_of_tokens[def_step][0] == ';')
		{
			def_flag=1;
			line_flag=1;
			def_step=0 ;
			return ;
		}	  
		else 
		{	
			line_flag=0;
			def_step=0;
			def_flag=0;
			return ; 	
		}

	}	
}



void is_an_operation()
{
	opl_flag=0;
	if (op_step==0)
	{ 
	op_flag=0;
	 
		for(int i= 0; i <=user_def_idex; i++)
			{
				if(strcmp(string_of_tokens[op_step],user_def[i]) == 0)
				{
					//printf("\n is d \n") ;
					op_step ++ ;
					op_flag=1;
					break ;
					
				}
				
				
			} 
			if (op_flag==0)
			{
				op_flag=0;
				op_step=0;

				return  ;
				
			}
			
			
		
	}

	if 	(op_step==1)
	{
		op_flag=0;
		//printf("\n is d \n") ;
		if (string_of_tokens[op_step][0]=='=')
		{
			op_flag=1;
			op_step++;
			//printf(" is d") ;
			
			 
			}
			
		else 
		{
			op_flag=0;
			op_step=0;
			return ; 
			
			
			
		}
		
	}	
			
		
	if 	(op_step==2)
	{
	int i=0; 
	int j=0; 
	op_flag=0;
	 
		for(int i= 0; i <=user_def_idex; i++)
			{
				if(strcmp(string_of_tokens[op_step],user_def[i]) == 0)
				{
					//printf("\n is d \n") ;
					op_step ++ ;
					op_flag=1;
					break ;
				}
				
				
			} 

	if (op_flag==0)
	{

		for(  j= 0 ; j < strlen(string_of_tokens[op_step]); j++ )
			{
			
				if ( isdigit(string_of_tokens[op_step][j])||string_of_tokens[op_step][j]=='.')
				{ 
					
					op_flag=1;
				   if(string_of_tokens[op_step][j]=='.')
					   dot_flag ++ ;
		
				}
				else 
				{
				op_flag=0;
				op_step=0;
				return ;
					
				}
				if(dot_flag>1)
					{
					
					op_step=0;
					op_flag=0;
					return ; 
						
					}
				
			}
			if(op_flag==1)
			{
				op_step++;
			}
			else 
			{
				   op_step=0;
					op_flag=0;
					return ; 
				
			}
	}
			
					
			
	}
	if 	(op_step==3)
	{
		op_flag=0;
		 
		if(string_of_tokens[op_step][0]=='+'||'-'||'/'||'*'||'^')
		{
			
			op_step++;
			op_flag=1;
		}
		else 
		{
				
			op_step=0;
			op_flag=0;
			return ; 
		}
			}

		
	if 	(op_step==4)
	{
	int i=0; 
	int j=0; 
	op_flag=0;
	 
		for(int i= 0; i <=user_def_idex; i++)
			{
				if(strcmp(string_of_tokens[op_step],user_def[i]) == 0)
				{
					
					
					//printf("\n is d \n") ;
					op_step ++ ;
					op_flag=1;
					break ;
				}
				
				
			} 

	if (op_flag==0)
	{

		for(  j= 0 ; j < strlen(string_of_tokens[op_step]); j++ )
			{
			
				if ( isdigit(string_of_tokens[op_step][j])||string_of_tokens[op_step][j]=='.')
				{ 
					
					op_flag=1;
				   if(string_of_tokens[op_step][j]=='.')
					   dot_flag ++ ;
		
				}
				else 
				{
				op_flag=0;
				op_step=0;
				return ;
					
				}
				if(dot_flag>1)
					{
					
					op_step=0;
					op_flag=0;
					return ; 
						
					}
				
			}
			if(op_flag==1)
			{
				op_step++;
			}
			else 
			{
				   op_step=0;
					op_flag=0;
					return ; 
				
			}
	}
			
					
			
	}


	if 	(op_step==5)
	{
		op_flag=0;
		opl_flag=0;
	if (string_of_tokens[op_step][0] == ';')
		{ 
		
		op_flag=1;
		 opl_flag=1;
		 op_step=0 ;
		  return ; }
		  
		  
		  
		else 
		{
			op_step=0;
			op_flag=0;
			opl_flag=0;
			printf("\nsyntax Error :( :( check your code and come back") ;
			exit(1);
			return ; 
			
		}

	}
}	
	
	
	
	




