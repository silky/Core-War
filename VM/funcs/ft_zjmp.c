/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_ldi.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nsabbah <nsabbah@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/20 17:14:24 by nsabbah           #+#    #+#             */
/*   Updated: 2017/03/24 17:25:40 by nsabbah          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../srcs/vm.h"

void	ft_zjmp(t_env *e, t_cursor *cursor)
{
	char	index_extra;

	if(cursor->carry)
	{
		index_extra = e->a[cursor->index + 1].hex;
		cursor->index = cursor->index + MODX(index_extra);
		ft_update_cursor(e, cursor, 2);
	}
	else
		ft_update_cursor(e, cursor, 1);
}
