/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iharchi <iharchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/15 17:13:57 by iharchi           #+#    #+#             */
/*   Updated: 2021/03/03 10:25:05 by iharchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdlib.h>
#include <unistd.h>
#include <stdarg.h>
#include <stdio.h>
/*
* ft_printf made in preparation to the exam hopefully i'll validated this time :)
*/
typedef struct  s_flag
{
    char con;
    int width;
    int has_prec;
    int prec;
}
               t_flag;
int ft_is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    return (0);    
}

int     ft_atoip(const char *fmt, int *i)
{
    int ret;

    ret = 0;
    while (ft_is_digit(fmt[*i]))
    {
        ret *= 10;
        ret += fmt[*i] - '0';
        *i += 1;
    }
    return (ret);
}

t_flag  get_flags(const char *fmt, int *i)
{
    t_flag flags = {0,0,0,0};

    while (fmt[*i])
    {
        if (ft_is_digit(fmt[*i]))
        {
            if (flags.has_prec)
                flags.prec = ft_atoip(fmt, i);
            else
                flags.width = ft_atoip(fmt, i);
        }
        if (fmt[*i] == '.')
            flags.has_prec = 1;
        if (fmt[*i] == 's' || fmt[*i] == 'd' || fmt[*i] == 'x')
        {
            flags.con = fmt[*i];
            break ;
        }
        if (fmt[*i])
            *i += 1;
    }
    return (flags);
}

int get_nbr_len(unsigned long long d, int div)
{
    unsigned long long tmp;
    int len;
    
    len = 0;
    tmp = d;
    while (tmp)
    {
        tmp /= div;
        len++;
    }
    return (len);
}

int printd(t_flag flags, int d)
{
    int len;
    int count;
    unsigned int tmp;
    char *p;
    int signe;

    signe = 1;
    if (d < 0)
        signe = -1;
    tmp = signe * d;
    count = 0;
    len = get_nbr_len((unsigned long long)tmp, 10);
    if (len == 0 && !flags.has_prec)
        len += 1;
    flags.width -= len;
    if (signe < 0)
        flags.width--;
    flags.prec -= len;
    if (flags.prec > 0)
        flags.width -= flags.prec;
    while (flags.width-- > 0)
        count += write (1, " ", 1);
    if (signe < 0)
        count += write(1, "-", 1);
    while (flags.has_prec && flags.prec-- > 0)
        count += write (1, "0", 1);
    if (tmp == 0 && !flags.has_prec)
    {
        count += write (1, "0", 1);
        return(count);
    }
    p = (char *)malloc(len + 1);
    p[len--] = '\0';
    while (tmp)
    {
        p[len--] = '0' + tmp % 10;
        tmp /= 10;
    }
    len = 0;
    while (p[len])
        count += write (1, &p[len++], 1);
    free(p);
    return (count);
}

int printx(t_flag flags, unsigned long long d)
{
    int len;
    int count;
    unsigned int tmp;
    char *p;
    int signe;

    tmp = d;
    count = 0;
    signe = 1;
    len = get_nbr_len((unsigned long long)tmp, 16);
    if (len == 0 && !flags.has_prec)
        len += 1;
    flags.width -= len;
    flags.prec -= len;
    if (flags.prec > 0)
        flags.width -= flags.prec;
    while (flags.width-- > 0)
        count += write (1, " ", 1);
    while (flags.has_prec && flags.prec-- > 0)
        count += write (1, "0", 1);
    if (tmp == 0 && !flags.has_prec)
    {
        count += write (1, "0", 1);
        return(count);
    }
    p = (char *)malloc(len + 1);
    p[len--] = '\0';
    while (d)
    {
        if (d % 16 < 10)
            p[len--] = '0' + d % 16;
        else
            p[len--] = 'a' + ((d % 16) - 10);
        d /= 16;
    }
    len = 0;
    while (p[len])
        count += write (1, &p[len++], 1);
    free(p);
    return (count);
}

int ft_strlen(char *s)
{
    int len;
    
    len = 0;
    while (s[len])
        len++;
    return (len);
}

char    *handle_null()
{
    char *s;

    s = (char *)malloc(7);
    s[0] = '('; 
    s[1] = 'n'; 
    s[2] = 'u'; 
    s[3] = 'l'; 
    s[4] = 'l'; 
    s[5] = ')'; 
    s[6] = '\0';
    return (s); 
}

int prints(t_flag flags, char *s)
{
    int len;
    int count;
    int f_free;
    int i;

    count = 0;
    f_free = 0;
    i = 0;
    if (!s)
    {
        f_free = 1;
        s = handle_null();
    }
    len = ft_strlen(s);
    if (flags.has_prec && flags.prec < len)
        len = flags.prec;
    flags.width -= len;
    while (flags.width-- > 0)
        count += write (1, " ", 1);
    while (i < len)
        count += write (1, &s[i++], 1);
    if (f_free)
        free(s);
    return (count);
}

int parse(const char *fmt, va_list args)
{
    int count;
    int i;
    t_flag flags;

    i = 0;
    count = 0;
    while (fmt[i])
    {
        if (fmt[i] != '%')
            count += write (1, &fmt[i], 1);
        else
        {
            flags = get_flags(fmt, &i);
            if (flags.con == 'd')
                count += printd(flags, va_arg(args, int));
            else if(flags.con == 'x')
                count += printx(flags, va_arg(args, unsigned long long));
            else if (flags.con == 's')
                count += prints(flags, va_arg(args, char *));
        }
        if (fmt[i])
            i++;
    }
    return (count);
}

int ft_printf(const char *fmt, ... )
{
    int count;
    va_list args;
    va_start(args, fmt);
    count = parse(fmt, args);
    va_end(args);
    return (count);
}
