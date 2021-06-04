/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   gnl.h                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: iharchi <iharchi@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/01/21 11:09:59 by iharchi           #+#    #+#             */
/*   Updated: 2021/01/21 11:36:09 by iharchi          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

# ifndef GNL
# define GNL
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <fcntl.h>

int get_next_line(char **line);
# endif