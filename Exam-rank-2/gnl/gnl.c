/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iharchi <iharchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 16:23:12 by iharchi           #+#    #+#             */
/*   Updated: 2021/01/21 16:58:15 by iharchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "gnl.h"
#define BSIZE 128

int ft_printf(char *fmt, ... );

int get_len(char *s)
{
    int i;

    i = 0;
    while (s[i])
        i++;
    return (i);
}

char *join_buff(char *buff, char *tmp)
{
    int i;
    int len;
    char *stmp;
    int len2;
    int j;
    
    len = 0;
    if (buff)
        len = get_len(buff);
    stmp = buff;
    len2 = get_len(tmp);
    buff = (char *)malloc(len + len2 + 1);
    i = 0;
    if (stmp)
    {

        while (stmp[i])
        {
            buff[i] = stmp[i];
            i++;
        }
        free(stmp);
    }
    j = 0;
    while (tmp[j])
    {
        buff[i] = tmp[j];
        j++;
        i++;
    }
    buff[i] = '\0';
    return (buff);
}

int find_pos(char *s, char c)
{
    int i;

    i = 0;
    while (s[i])
    {
        if (s[i] == c)
            return (i);
        i++;
    }
    return (-1);
}

char *get_before(char *buff, int pos)
{
    int i;
    char *ret;
    
    ret = (char *)malloc(pos + 1);
    i = 0;
    while (i < pos)
    {
        ret[i] = buff[i];
        i++;
    }
    ret[i] = '\0';
    return (ret);
}
char *get_after(char *buff, int pos)
{
    int i;
    int len;
    char *tmp;
    
    len = get_len(buff);
    tmp = buff;
    pos++;
    buff = (char *)malloc(len - pos);
    i = 0;
    while (tmp[i + pos])
    {
        buff[i] = tmp[i + pos];
        i++;
    }
    buff[i] = '\0';
    return (buff);
}

int get_next_line(char **line)
{
    int n;
    static char *buff;
    char *tmp;
    int pos;
    
    if (buff)
    {
        if ((pos = find_pos(buff, '\n')) >= 0)
        {
            *line = get_before(buff, pos);
            buff = get_after(buff, pos);
            printf("%s", buff);
            return (1);
        }
    }
    tmp = (char *)malloc(BSIZE + 1);
    while ((n = read(0, tmp, BSIZE)))
    {
        buff = join_buff(buff, tmp);
        if ((pos = find_pos(buff, '\n')) >= 0)
        {
            *line = get_before(buff, pos);
            buff = get_after(buff, pos);
            break ;
        }
    }
    return (n > 0 ? 1 : n);
}