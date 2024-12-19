/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   kitty.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: codespace <codespace@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/19 23:31:25 by cgorin            #+#    #+#             */
/*   Updated: 2024/12/19 17:23:38 by codespace        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_ascii_art_p1(void)
{
	printf(C"              ⣀⣀⣀⡀                          ⣀⣤⣄         \n");
	printf(B"            ⢠⡾⠋⠉⠉⠉⠛⢶⣄⣀⣀⣀⣤⣤⣤⣤⣤⢤⣤⣤⣤⣤⣀⣀ ⣠⣶⣿⣿⣿⣶⣟⠉ ⠙⢷⣄    \n");
	printf(R"           ⢠⡟       ⠈⠛⢯⣅          ⠈⢉⣿⣿⡿⣯⣷⣻⣽⣿⣷   ⠹⣧      \n");
	printf(G"          ⢀⡿           ⠈⠳⠂         ⣾⣿⣯⣟⣷⢯⣟⣾⣳⣿⣧⣤⣄⡀⠙⣇     \n");
	printf(Y"        ⢀⡴⢿⠃                       ⣿⣿⢾⣽⣾⣿⣿⣿⣿⡿⣿⣻⢿⣿⣿⣿⣿⣆   \n");
	printf(P"       ⣴⠟⠁⣿                        ⣿⣿⣟⣾⣽⣿⣿⣿⣯⣟⡷⣿⢯⣟⣿⣿⣯⣿⡆  \n");
	printf(C"     ⢀⡾⠁ ⢀⡟                        ⠸⣿⣟⣾⡽⣿⣿⣿⣾⣽⣻⣽⣯⢿⣿⣿⣿⣿⡇  \n");
	printf(R"     ⣾⠁  ⢸⡇                         ⢹⣿⣯⣿⢳⣿⣿⣿⣾⣿⣾⣽⣿⣿⣿⡟⣿⡇  \n");
	printf(G"    ⣼⠃   ⢸⡇                          ⠉⠛⠛⠛⠛⠁ ⠉⠛⠻⣿⣿⣻⣽⣿⡟   \n");
	printf(Y"   ⢸⡏    ⠈⠃          ⣀⣤⠶⠶⠲⢶⣄                   ⠘⠻⠿⢿⡋    \n");
	printf(B"   ⣿               ⢠⡾⠋     ⠹⣇                      ⣧    \n");
	printf(P"⣀⣀⢰⡇              ⣴⠟        ⢻⡄                     ⣿⢀⢀⣀⣀\n");
	printf(C"⠉⠉⢹⡟⠛⠓⠶⠦⣤⣤⣀⣀                ⠘⣇                  ⣤⠶⠞⣿⠛⠉⠉⠉\n");
	printf(R"  ⠸⣇      ⢈⣉⠛⠓⠶         ⣠⣄   ⢻⣼⡇        ⢰⡇  ⣀⣤⠆    ⡧    \n");
	printf(G"   ⣿       ⠻⣷⣄        ⢀⣾⣿⣿⣿⡄ ⠈⠻⠃        ⠈⢷⣾⣿⣿⡇  ⠶⠛⢻⠟⠲⠶⠆ \n");
	printf(Y"   ⠸⣇    ⣀⣀⣤⡼⢿⣷⡀      ⢨⣿⣿⣿⣿⡧             ⠈⣿⣿⣿⠇   ⢀⡿     \n");
	printf(B"    ⣻⣦⠶⠞⠛⠉⠉  ⠈⠻⣿⣦⡀     ⣿⣿⣿⣿⡇              ⠈⠛⠋  ⠰⢶⣾⣥⣄⡀   \n");
	printf(P" ⠠⠶⠛⠉⠹⣦       ⣠⡿⢿⣿⣦⡀   ⠙⢿⡿⠟        ⣶⠛⠻⣶        ⣤⠿⠛⠋⠛⠛⢷⣄ \n");
	printf(C"      ⠘⢷⡄  ⢀⣠⠞⠋ ⠈⠻⣿⣿⣦              ⠙⠛⠛⠉     ⣀⣴⣾⠃      ⠹⣆\n");
}

void	print_ascii_art_p2(void)
{
	printf(R"        ⠹⢦⣴⠟⠁     ⠙⢿⣿⣿⣤              ⣀⣀⣀⣤⣴⣶⣿⣿⣿⡟        ⣿\n");
	printf(G"        ⣰⠟⠙⠳⢦⣄⡀    ⠈⠻⣿⣿⣿⣿⣿⣿⣿⣷⣶⣶⣾⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡏        ");
	printf("⣿\n");
	printf(Y"      ⢀⡾⠃    ⠈⠙⠲⢦⣤⣄⣀⡀⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⢃⣾⣿⣿⣿⣿⣿⣿⠿⣿⣿⣿⣿⣿⠟⠁");
	printf(B"       ⢰⡏\n               ⣴⠟⠁⠊⠉⢹⣿⣿⣿⣿⣿⣿⣿⣿⣿⣧⣿⣿⣿⣿⣿⣿⣿⣯⠳⣍⢿⣿⣿⠏ ");
	printf("       ⢀⡾⠁\n");
	printf(P"             ⢠⡾⠃    ⣸⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⢆⡻⡜⢯⢼⣿⡆       ⣠⡞⠁\n");
	printf(C"            ⣠⡟⠁     ⢾⣿⡿⢿⣿⣿⣿⣿⣿⣿⣿⣿⣿⡿⣛⢧⡹⢎⡵⣙⡎⡞⡽⢿⣶⣄⣀⣀⣀⣤⠾⠋  \n");
	printf(R"          ⣰⣿⣤⣄⣀⡀   ⢺⣿⢱⢫⡜⣭⢋⡟⣭⢛⡭⢫⡕⢮⡱⢭⡲⣙⣮⡜⡷⢪⡝⣜⣻⡿ ⠉⠉⠉   \n");
	printf(G"          ⢠⡟  ⠈⠉⠙⠛⠻⠶⣾⣿⢬⠳⣜⠲⣭⢚⡴⢫⡜⡳⣜⡣⡝⢶⣩⢳⡮⢼⡳⢫⡜⣮⣿⠃     \n");
	printf(Y"          ⣿         ⠘⢿⣟⡧⣜⡻⣄⠿⡸⢇⡼⢣⡜⣣⢻⢤⣃⡟⣸⢇⡻⢇⡼⣿⡿       \n");
	printf(B"         ⠐⣏           ⣿⡷⣌⡳⣜⢣⢏⡽⣘⢧⡹⢲⢭⡒⢧⣚⠵⣪⠷⣋⣾⡿⠁       \n");
	printf(P"          ⣿        ⣴⣤⣾⣟⡱⢎⡵⢎⡝⣎⠶⣍⠶⣍⡳⢎⣝⢣⡎⢷⣡⢿⣼⣿⠇        \n");
	printf(C"          ⠙⣧⡀    ⢀⣼⡿⢎⡵⢪⡕⣫⡜⣳⢚⣬⢓⡎⡗⢮⣱⢫⡜⣫⢞⣱⣾⣿⣿⣿⠇       \n");
	printf(R"           ⠈⠙⠳⠶⠶⢾⣿⡛⡵⢫⣜⢣⢞⡱⢎⡵⣋⠶⣩⢞⡹⢎⣖⣣⣽⣿⣿⣿⣿⡿⡟⣻⠇      \n");
	printf(G"                ⠘⣿⡵⣋⠷⣌⢏⠮⣕⡫⢖⡹⢎⡵⣎⣷⣿⣾⢿⣿⡿⠟⠛⠉ ⣷⠟             \n");
	printf(Y"                 ⠸⣿⡵⢫⡜⢮⣙⠦⣝⢣⡝⢮⡙⠿⣭⣳⣿ ⢻⣦⣤⣤⠶⠛⠁              \n");
	printf(B"                  ⠙⣿⣷⣾⣧⣮⣽⣬⣷⣾⣧⣿⠿⠟⢻⡇                     \n");
	printf(P"                   ⠈⠳⣄ ⠉⠉⠉⠁⠁   ⣠⡟                       \n");
	printf(C"                     ⠈⠛⠶⠶⠶⠶⠶⠶⠚⠛⠉                         \n");
	printf(RESET);
}

void	ft_kitty(void)
{
	print_ascii_art_p1();
	print_ascii_art_p2();
}
