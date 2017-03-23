/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   set_hexa_instruction.c                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mhaziza <mhaziza@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2017/03/18 14:45:44 by mhaziza           #+#    #+#             */
/*   Updated: 2017/03/22 19:37:28 by mhaziza          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "asm.h"

int			set_opcode(t_asm tasm, char *line, int fd, int *cursor)
{
	int		id;

	if (!(id = get_id_by_name(&tasm, line)))
		return (0);
	ft_putchar_fd(id, fd);
	*cursor += 1;
	return (id);
}

void		set_acb(t_op top, char *line, int fd, int *cursor)
{
	unsigned char	acb;
	int				i;
	int				j;

	i = -1;
	acb = 0;
	j = 0;
	while (++i < top.nb_params)
	{
		if (line[0] == 'r')
			acb = acb + (REG_CODE << ((3 - i) * 2));
		else if (line[0] == '%')
			acb = acb + (DIR_CODE << ((3 - i) * 2));
		else
			acb = acb + (IND_CODE << ((3 - i) * 2));
		line = ft_strchr(line, SEPARATOR_CHAR) ?
		ft_strchr(line, SEPARATOR_CHAR) + 1 : line;
	}
	ft_putchar_fd(acb, fd);
	*cursor += 1;
}

int			begin_by_label(char *line)
{
	int	i1;
	int	i2;
	int	i3;

	i1 = ft_strchr(line, LABEL_CHAR) ? ft_strchr(line, LABEL_CHAR) - line : -1;
	i2 = ft_strchr(line, ' ') ? ft_strchr(line, ' ') - line : -1;
	i3 = ft_strchr(line, '\t') ? ft_strchr(line, '\t') - line : -1;
	if (i1 > -1 && (i2 > -1 || i3 > -1) && (i1 < i2 || i1 < i3))
		return (1);
	return (0);
}

static void	set_params(t_asm tasm, t_write *tw, char **tab, int fd)
{
	int	j;
	int	count;

	count = -1;
	j = 0;
	while (tab[tw->is_label] &&
		++count < tasm.op_tab[tw->id_instruction].nb_params)
	{
		if (tab[1 + tw->is_label][j] == 'r')
			set_register(tab[1 + tw->is_label] + j + 1,
				fd, &tw->cursor);
		else if (tab[1 + tw->is_label][j] == '%')
			set_direct(tasm, tasm.op_tab[tw->id_instruction],
				tab[1 + tw->is_label] + j + 1, fd, &tw->cursor,
				tw->instruction_cursor);
		else
			set_indirect(tasm, tab[1 + tw->is_label] + j, fd, &tw->cursor,
				tw->instruction_cursor);
		while (tab[1 + tw->is_label][j] &&
			tab[1 + tw->is_label][j] != SEPARATOR_CHAR)
			j++;
		j++;
	}
	tw->is_label = 0;
}

int			set_instruction(t_asm tasm, int fd, int nb_line)
{
	int		i;
	t_write	tw;
	char	**tab;

	ft_bzero(&tw, sizeof(t_write));
	i = 2;
	tw.cursor = -1;
	while (++i < nb_line)
	{
		tab = tasm.asm_master[i];
		if (tab && tab[0] && tab[0][0] && tab[0][0] != COMMENT_CHAR)
		{
			if (ft_strchr(tab[0], LABEL_CHAR))
				tw.is_label = 1;
			if (tab[tw.is_label] && !(tw.id_instruction =
				set_opcode(tasm, tab[tw.is_label], fd, &tw.cursor)))
				return (0);
			tw.instruction_cursor = tw.cursor;
			if (tab[tw.is_label] && tasm.op_tab[tw.id_instruction].acb)
				set_acb(tasm.op_tab[tw.id_instruction],
					tab[1 + tw.is_label], fd, &tw.cursor);
			set_params(tasm, &tw, tab, fd);
		}
	}
	return (1);
}
