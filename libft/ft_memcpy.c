/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: zzhyrgal <zzhyrgal@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/11/11 19:45:37 by zzhyrgal          #+#    #+#             */
/*   Updated: 2025/11/11 21:24:01 by zzhyrgal         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest_str, const void *src_str, size_t n)
{
	int					i;
	unsigned char		*char_dest;
	const unsigned char	*char_src;

	if (dest_str == NULL && src_str == NULL)
		return (NULL);
	char_dest = (unsigned char *)dest_str;
	char_src = (const unsigned char *)src_str;
	i = 0;
	while (n-- > 0)
	{
		char_dest[i] = char_src[i];
		i++;
	}
	return (dest_str);
}
