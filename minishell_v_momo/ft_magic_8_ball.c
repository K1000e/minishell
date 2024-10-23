/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_magic_8_ball.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mabdessm <mabdessm@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/23 08:17:36 by mabdessm          #+#    #+#             */
/*   Updated: 2024/10/23 08:18:09 by mabdessm         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

void	decider(int i)
{
	if (i % 8 == 0)
		trollface();
	else if (i % 8 == 1)
		kappa();
	else if (i % 8 == 2)
		therock();
	else if (i % 8 == 3)
		mewing();
	else if (i % 8 == 4)
		jerma();
	else if (i % 8 == 5)
		chungus();
	else if (i % 8 == 6)
		despair();
	else if (i % 8 == 7)
		angy();
}

void	magiceightball(void)
{
	eightball();
    int i;

    printf("write down the first positive integer that comes to mind : ");
    while (1)
	{
        if (scanf("%d", &i) != 1 || i < 0)
		{
            while (getchar() != '\n');
            printf("Invalid input. Please enter a positive integer: ");
        }
		else
            break;
    }
	decider(i);
}

int	main()
{
	magiceightball();
}
