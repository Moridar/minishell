/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipex_split_shell_cmd.c                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bsyvasal <bsyvasal@student.hive.fi>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/22 15:11:39 by vshchuki          #+#    #+#             */
/*   Updated: 2024/01/27 19:22:38 by bsyvasal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "pipex.h"

/**
 * Replaces all the whitespaces inside the command to special char.
 * It should not affect whitespces inside quotes.
 * 
 * @return pointer to a new command string.
*/
char	*replace_spaces(char *cmd)
{
	int		i;
	char	*new_cmd;

	new_cmd = ft_strdup(cmd);
	if (!new_cmd)
		return (NULL);
	i = 0;
	while (new_cmd[i])
	{
		if (ft_isspace(new_cmd[i]))
			new_cmd[i] = 31;
		if ((new_cmd[i] == '\'' || new_cmd[i] == '"')
			&& get_quote_length(&new_cmd[i], new_cmd[i]) != 1)
			i += get_quote_length(&new_cmd[i], new_cmd[i]) - 1;
		i++;
	}
	return (new_cmd);
}

/**
 * Takes double array of splitted command and trims quotes and double quotes
 * from each element while expanding the environment variables.
*/
void	trim_expand_flag_quotes(char **command)
{
	char	*temp;
	int		i;

	i = 0;
	while (command[i])
	{
		temp = command[i];
		command[i] = expand_env_args(temp);
		free(temp);
		temp = command[i];
		command[i] = ft_strtrim(temp, (char []){'\'', '"'});
		free(temp);
		i++;
	}
}

/**
 * Splits the shell command. Preserves whitespaces inside quotes, expands env
 * variables and trims wrapping quotes.
 * If there were any redirections and filenames in the string, they should be 
 * removed and replaced with whitespaces before using the function.
 * 
 * Some kind of a smart make_args()
 * 
 * @return double dimension array of command and it's flags
*/
char	**split_shell_cmd(char	*cmd)
{
	char	*new_str;
	char	**command;
	int i;

	new_str = replace_spaces(cmd);
	command = ft_split(new_str, 31);
	free(new_str);
	i = 0;
	while (command[i])
	{
		new_str = command[i];
		command[i] = interpret(new_str);
		free(new_str);
		i++;
	}
	// trim_expand_flag_quotes(command);
	return (command);
}

/* int	main(void)
{
	char	*cmd;
	char	**command;
	int i;

	// cmd = "    	  echo 	   \" \'  \' word \' hello 	 $SHELL  $MAIL \"       \'h world $SHELL \" h\'   $MAIL $MAIL $NOT_EXISTS  ";
	// cmd = "echo \' something should happen here\"\"";
	// cmd = "\"hello\"";
	// cmd = "    	  echo 	   \" \'  \' word \' hello 	 $SHELL  $MAIL \"       \'h world $SHELL \" h\'   $MAIL $MAIL $NOT_EXISTS  ";
	// cmd = "'";
	// cmd = "echo $SHELL\"HEY\" $SHELL' hey' \"$SHELL 'HEY' HEY$SHELL\" \'hey\' \"hello\" \"' hey hey '\"";
	
	// cmd = "echo \"$SHELL\""; // works!
	// cmd = "echo \"$SHELL\" "; // works!
	// cmd = "echo \"$SHELL \""; // gives an empty string
	cmd = "echo \"$SHELL \"\"\""; // gives an empty string
	// cmd = "echo \"$SHELL \"\'\'"; // gives an empty string
	// cmd = "echo \"$SHELL 'HEY'\""; // gives an empty string
	// cmd = "echo \"$SHELL 'HEY' HEY$SHELL\""; // gives an empty string
	
	// printf("cmd: %s\n", cmd);

	// in bash:
	//     	  echo 	   " '  ' word ' hello 	 $SHELL  $MAIL "       'h world $SHELL " h'   $MAIL $MAIL $NOT_EXISTS  
	
	command = split_shell_cmd(cmd);
	printf("Array elements:\n");
	i = 0;
	while(command[i])
	{
		printf("|%s|\n", command[i]);
		i++;
	}
	freeall(command);

	return (0);
} */
