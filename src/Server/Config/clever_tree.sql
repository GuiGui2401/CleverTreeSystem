-- phpMyAdmin SQL Dump
-- version 5.1.1
-- https://www.phpmyadmin.net/
--
-- Hôte : 127.0.0.1:3306
-- Généré le : mar. 21 mars 2023 à 12:34
-- Version du serveur : 5.7.36
-- Version de PHP : 7.4.26

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
START TRANSACTION;
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Base de données : `clever_tree`
--

-- --------------------------------------------------------

--
-- Structure de la table `module`
--

DROP TABLE IF EXISTS `module`;
CREATE TABLE IF NOT EXISTS `module` (
  `id` int(7) NOT NULL AUTO_INCREMENT COMMENT 'Identifiant par enregistrement',
  `Data_PumpStatus` int(2) NOT NULL DEFAULT '0' COMMENT 'Position: 0\r\néteint ou allumé',
  `Data_TankLevel` int(7) NOT NULL DEFAULT '0' COMMENT 'Position: 1\r\nUnité: %',
  `Data_SoilMosture_01` int(7) NOT NULL DEFAULT '0' COMMENT 'Position: 2\r\nUnité: %',
  `Data_SoilMosture_02` int(7) NOT NULL DEFAULT '0' COMMENT 'Position: 3\r\nUnité: %',
  `Data_SoilMosture_03` int(7) NOT NULL DEFAULT '0' COMMENT 'Position: 4\r\nUnité: %',
  `Data_Temperature` float NOT NULL DEFAULT '0' COMMENT 'Position: 5\r\nUnité: °C',
  `Data_Humidity` float NOT NULL DEFAULT '0' COMMENT 'Position: 6\r\nUnité: %',
  `Data_WindSpeed` float NOT NULL DEFAULT '0' COMMENT 'Position: 7\r\nUnité: KM/H',
  `Data_InfraRed` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 8',
  `Data_Ultraviolet` float NOT NULL DEFAULT '0' COMMENT 'Position: 9 ',
  `Data_Visible` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 10',
  `Data_CarbonMonoxyd` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 11',
  `Data_LPG` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 12',
  `Data_Smoke` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 13',
  `PS_PumpStatus` int(1) NOT NULL DEFAULT '0' COMMENT 'Position: 0 Eteint ou Allumé',
  `PS_TankLevel` int(9) NOT NULL DEFAULT '0' COMMENT 'Position: 1 Unité: %',
  `SD_Temperature` int(6) NOT NULL DEFAULT '0' COMMENT 'Position: 0 Unité: °C',
  `SD_Humidity` int(9) NOT NULL DEFAULT '0' COMMENT 'Position: 1 Unité: %',
  `SD_Infrared` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 2',
  `SD_Ultraviolet` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 3',
  `SD_Visible` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 4',
  `SD_CarbonMonoxyd` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 5',
  `SD_LPG` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 6',
  `SD_Smoke` int(255) NOT NULL DEFAULT '0' COMMENT 'Position: 7',
  `WS_WindSpeed` int(9) NOT NULL DEFAULT '0' COMMENT 'Position: 0 Unité: KM/H',
  `SM_SoilMoisture_01` int(9) NOT NULL DEFAULT '0' COMMENT 'Position: 2 Unité: %',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM AUTO_INCREMENT=12 DEFAULT CHARSET=latin1;

--
-- Déchargement des données de la table `module`
--

INSERT INTO `module` (`id`, `Data_PumpStatus`, `Data_TankLevel`, `Data_SoilMosture_01`, `Data_SoilMosture_02`, `Data_SoilMosture_03`, `Data_Temperature`, `Data_Humidity`, `Data_WindSpeed`, `Data_InfraRed`, `Data_Ultraviolet`, `Data_Visible`, `Data_CarbonMonoxyd`, `Data_LPG`, `Data_Smoke`, `PS_PumpStatus`, `PS_TankLevel`, `SD_Temperature`, `SD_Humidity`, `SD_Infrared`, `SD_Ultraviolet`, `SD_Visible`, `SD_CarbonMonoxyd`, `SD_LPG`, `SD_Smoke`, `WS_WindSpeed`, `SM_SoilMoisture_01`) VALUES
(1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123456, 56, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123456, 56, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 123456, 56, 0, 97, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(5, 1, 2, 3, 0, 0, 0.2, 0.5, 0.7, 0, 0.567, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(6, 1, 2, 3, 0, 0, 0.2, 0.5, 0.7, 9, 0.567, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(7, 1, 2, 3, 0, 0, 0.23, 0.5, 0.7, 9, 0.567, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(8, 1, 2, 3, 0, 0, 0.23, 0.56, 0.75, 9, 0.5678, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(9, 1, 2, 3, 0, 0, 0.23, 1.56, 0.75, 9, 0.5678, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(10, 1, 2, 3, 0, 0, 0.23, 1.56, 0.75, 9, 0.5678, 6, 7, 8, 9, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0),
(11, 12, 23, 34, 0, 0, 12.23, 19.56, 135.75, 98, 9999.57, 678, 79, 80, 90, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0);
COMMIT;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
