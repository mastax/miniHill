/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libft_utils4.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: elel-bah <marvin@42.fr>                    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/12 20:48:52 by elel-bah          #+#    #+#             */
/*   Updated: 2024/09/12 20:48:53 by elel-bah         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../mini_shell.h"

static int	numwords(char const *s, char c)
{
	int	cur;
	int	word_num;

	cur = 0;
	word_num = 0;
	while (s[cur] != 0)
	{
		if (s[cur] != c && (s[cur + 1] == c || s[cur + 1] == 0))
			word_num++;
		cur++;
	}
	return (word_num);
}

static int	split_words(char **result, char const *s, char c, int word)
{
	int		start_cur;
	int		end_cur;

	end_cur = 0;
	start_cur = 0;
	while (s[end_cur])
	{
		if (s[end_cur] == c || s[end_cur] == 0)
			start_cur = end_cur + 1;
		if (s[end_cur] != c && (s[end_cur + 1] == c || s[end_cur + 1] == 0))
		{
			result[word] = malloc(sizeof(char) * (end_cur - start_cur + 2));
			if (!result[word])
			{
				while (word++)
					free(result[word]);
				return (0);
			}
			ft_strlcpy(result[word], (s + start_cur), end_cur - start_cur + 2);
			word++;
		}
		end_cur++;
	}
	result[word] = 0;
	return (1);
}

char	**ft_split(char const *s, char c)
{
	char	**result;

	if (!s)
		return (NULL);
	result = malloc(sizeof(char *) * (numwords(s, c) + 1));
	if (!result)
		return (NULL);
	if (!split_words(result, s, c, 0))
		return (NULL);
	return (result);
}

int	ft_isalpha(int c)
{
	return ((c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z'));
}

char *ft_strtok(char *str, const char *delim) {
    static char *stock = NULL;
    char *ptr;
    int flg = 0;

    if (str != NULL) {
        free(stock);  // Free previous allocation
        stock = ft_strdup(str);
        if (stock == NULL) return NULL;  // Check for allocation failure
    } else if (stock == NULL) {
        return NULL;  // Nothing to tokenize
    }

    ptr = stock;
    while (*stock != '\0') {
        if (flg == 0 && !ft_strchr(delim, *stock)) {
            flg = 1;
            ptr = stock;
        } else if (flg == 1 && ft_strchr(delim, *stock)) {
            *stock = '\0';
            stock++;
            return ptr;
        }
        stock++;
    }

    if (flg) {
        stock = NULL;  // Indicate end of tokenization
        return ptr;
    }

    free(stock);  // Free if no token found
    stock = NULL;
    return NULL;
}

// char	*ft_strtok(char *str, const char *delim)
// {
// 	static char	*stock;
// 	char		*ptr;
// 	int			flg;

// 	flg = 0;
// 	ptr = NULL;
// 	stock = NULL;
// 	if (str != NULL)
// 		stock = ft_strdup(str);
// 	while (*stock != '\0')
// 	{
// 		if (flg == 0 && !ft_strchr(delim, *stock))
// 		{
// 			flg = 1;
// 			ptr = stock;
// 		}
// 		else if (flg == 1 && ft_strchr(delim, *stock))
// 		{
// 			*stock = '\0';
// 			stock += 1;
// 			break ;
// 		}
// 		stock += 1;
// 	}
// 	return (ptr);
// }
