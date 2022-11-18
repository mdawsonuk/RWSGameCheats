<p align="center">
  <h2 align="center">Real World Systems Game Cheats</h3>
	
  <p align="center">
  <a href="https://github.com/mdawsonuk/RWSGameCheats/actions">
		<img src="https://github.com/mdawsonuk/RWSGameCheats/actions/workflows/compile.yml/badge.svg" alt="GitHub Actions Status (Compile)"/></a>
  </p>
  <p align="center">
    Security of Real World Systems Coursework 2 · "The One Where We Hack A Game"
    <br/>
    Matt Dawson · Harry Stoltz · Jay Newey · David-Daniel Candreanu · Daniel-Stefan Stoica
    <br/>
    <a href="https://docs.google.com/document/d/13sCQu-smekV9LokajMxyDpV7qhvywv5qnrvJlmDjA7A/edit#heading=h.fmyxpvr5v3v7">Report</a> · <a href="https://canvas.bham.ac.uk/courses/65671/assignments/381675">Canvas Page</a>
  </p>
</p>

## Table of Contents

* [About the Project](#about-the-project)
* [Usage](#usage)

## About The Project

This cheat is for Counter Strike: Global Offensive, a free-to-play multiplayer first-person shooter. This cheat is an adaptive difficulty cheat, which uses the player's kills, assists and deaths to determine which cheats or reverse cheats to enable or disable.

The following cheats/reverse cheats are present in our project
* Aimbot - this snaps the player’s aim to the closest enemy’s head
* BHop - this enables the player to hold down the spacebar and jump again the moment they land on the floor, rather than having to press the spacebar repeatedly
* No jump - this prevents the player from jumping at all
* Chams - this allows you to view the enemy players through walls, as well as changing their colour depending on if they are occluded from view or not
* Glow - this puts a flashing outline around different entities in the game world. Green signifies the entity is friendly, red signifies the entity is a hostile enemy, and blue is for other types of entities such as discarded or dropped weapons
* No Flash - this removes the flashbang effect from the player when they look at a flashbang so they aren’t temporarily blinded
* No Recoil - this removes all recoil from the player's weapons. This is combined with aimbot to make aimbot work more effectively
* Skin changer - this switches the player’s models, skins and animations to ones which are pay-to-win items

## Usage

To use the cheat, you need to use [`Extreme Injector v3.exe`](blob/main/Extreme%20Injector%20v3.exe) to inject the cheat. The `csgo.exe` process needs to be selected, and the DLL `csgo-internal.dll` needs to be added to the inject list. The cheat can be downloaded from the [releases page](/releases)

Upon the cheat being injected, two windows will pop up - the debug console and the cheat controlling window.

The debug console will output debug information and state changes, such as the player index, the player's stats and adaptive difficulty being enabled or disabled.

The cheat controlling window will enable you to toggle adaptive difficulty. If this is enabled, the manual cheat buttons will be disabled. Otherwise, you can manually toggle cheats on and off.