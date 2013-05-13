-- phpMyAdmin SQL Dump
-- version 3.5.4
-- http://www.phpmyadmin.net
--
-- Host: 10.202.68.49
-- Generation Time: Apr 29, 2013 at 04:54 PM
-- Server version: 5.5.27
-- PHP Version: 5.3.3

SET SQL_MODE="NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8 */;

--
-- Database: `dbms`
--

-- --------------------------------------------------------

--
-- Table structure for table `qlms_book`
--

CREATE TABLE IF NOT EXISTS `qlms_book` (
  `isbn` int(11) NOT NULL,
  `title` char(50) NOT NULL,
  `type` char(50) NOT NULL,
  `pub_press` char(50) NOT NULL,
  `pub_year` int(11) NOT NULL,
  `author` char(20) NOT NULL,
  `price` double NOT NULL,
  `num_total` int(11) NOT NULL,
  PRIMARY KEY (`isbn`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `qlms_book`
--

INSERT INTO `qlms_book` (`isbn`, `title`, `type`, `pub_press`, `pub_year`, `author`, `price`, `num_total`) VALUES
(2, '西窗法雨', '普及读物', '院系分馆', 2008, '刘星', 25, 3),
(21, 'Computer Architecture', 'Computer Science', 'xxx', 2004, 'xxx', 90, 2),
(31, 'Computer Architecture', 'Computer Science', 'xxx', 2004, 'xxx', 90, 6),
(32, '编译原理', '计算机科学', 'xxx', 2008, 'xxx', 90, 9),
(33, '人工智能1', '计算机科学', 'xxx', 2004, 'xxx', 90, 2),
(34, '计算机网络1', '计算机科学', 'xxx', 2008, 'xxx', 90, 3),
(1111, '中国文学理论批评史教程', '教材', '西溪流通书库', 1999, '张少康', 25, 2);

-- --------------------------------------------------------

--
-- Table structure for table `qlms_book_item`
--

CREATE TABLE IF NOT EXISTS `qlms_book_item` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `isbn` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `location` char(20) NOT NULL,
  PRIMARY KEY (`id`),
  KEY `isbn` (`isbn`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=32 ;

--
-- Dumping data for table `qlms_book_item`
--

INSERT INTO `qlms_book_item` (`id`, `isbn`, `status`, `location`) VALUES
(1, 1111, 1, '西溪流通书库'),
(2, 1111, 1, '西溪流通书库'),
(3, 2, 1, '院系分馆'),
(4, 2, 1, '院系分馆'),
(5, 2, 1, '院系分馆'),
(6, 21, 1, '默认位置'),
(7, 21, 1, '默认位置'),
(12, 31, 1, '默认位置'),
(13, 31, 1, '默认位置'),
(14, 32, 1, '默认位置'),
(15, 32, 1, '默认位置'),
(16, 32, 1, '默认位置'),
(17, 31, 1, '默认位置'),
(18, 31, 1, '默认位置'),
(19, 32, 1, '默认位置'),
(20, 32, 1, '默认位置'),
(21, 32, 1, '默认位置'),
(22, 31, 1, '默认位置'),
(23, 31, 1, '默认位置'),
(24, 32, 1, '默认位置'),
(25, 32, 1, '默认位置'),
(26, 32, 1, '默认位置'),
(27, 33, 1, '默认位置'),
(28, 33, 1, '默认位置'),
(29, 34, 0, '默认位置'),
(30, 34, 0, '默认位置'),
(31, 34, 1, '默认位置');

-- --------------------------------------------------------

--
-- Table structure for table `qlms_record`
--

CREATE TABLE IF NOT EXISTS `qlms_record` (
  `uuid` int(11) NOT NULL AUTO_INCREMENT,
  `id` int(11) NOT NULL,
  `stuid` int(11) NOT NULL,
  `status` int(11) NOT NULL,
  `time_borrow` timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
  `time_return` timestamp NULL DEFAULT NULL,
  PRIMARY KEY (`uuid`),
  KEY `id` (`id`),
  KEY `stuid` (`stuid`)
) ENGINE=InnoDB  DEFAULT CHARSET=utf8 AUTO_INCREMENT=11 ;

--
-- Dumping data for table `qlms_record`
--

INSERT INTO `qlms_record` (`uuid`, `id`, `stuid`, `status`, `time_borrow`, `time_return`) VALUES
(1, 7, 2, 1, '2013-04-22 08:30:40', '2013-04-22 08:30:40'),
(2, 3, 2, 1, '2013-04-22 08:03:42', '2013-04-22 08:03:42'),
(3, 5, 1, 1, '2013-04-22 08:02:30', '2013-04-22 08:02:30'),
(4, 4, 2, 1, '2013-04-22 08:30:46', '2013-04-22 08:30:46'),
(5, 6, 1, 1, '2013-04-29 05:01:12', '2013-04-29 05:02:00'),
(6, 7, 1, 1, '2013-04-29 05:01:44', '2013-04-29 05:02:02'),
(7, 1, 1, 1, '2013-04-29 05:01:51', '2013-04-29 05:02:04'),
(8, 29, 2, 0, '2013-04-29 05:02:14', NULL),
(9, 30, 2, 0, '2013-04-29 05:02:15', NULL),
(10, 27, 1, 1, '2013-04-29 06:43:49', '2013-04-29 06:47:11');

-- --------------------------------------------------------

--
-- Table structure for table `qlms_user`
--

CREATE TABLE IF NOT EXISTS `qlms_user` (
  `stuid` int(11) NOT NULL,
  `password` char(32) NOT NULL,
  `name` char(20) NOT NULL,
  `num_borrowed` int(11) NOT NULL,
  `num_limit` int(11) NOT NULL,
  `department` char(20) NOT NULL,
  `isadmin` tinyint(1) NOT NULL,
  PRIMARY KEY (`stuid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

--
-- Dumping data for table `qlms_user`
--

INSERT INTO `qlms_user` (`stuid`, `password`, `name`, `num_borrowed`, `num_limit`, `department`, `isadmin`) VALUES
(1, '1', '管理员', 0, 100, '图书馆', 1),
(2, '123', '小明', 2, 2, '云峰学园', 0);

--
-- Constraints for dumped tables
--

--
-- Constraints for table `qlms_book_item`
--
ALTER TABLE `qlms_book_item`
  ADD CONSTRAINT `qlms_book_item_ibfk_1` FOREIGN KEY (`isbn`) REFERENCES `qlms_book` (`isbn`) ON DELETE CASCADE ON UPDATE CASCADE;

--
-- Constraints for table `qlms_record`
--
ALTER TABLE `qlms_record`
  ADD CONSTRAINT `qlms_record_ibfk_1` FOREIGN KEY (`id`) REFERENCES `qlms_book_item` (`id`) ON DELETE CASCADE ON UPDATE CASCADE,
  ADD CONSTRAINT `qlms_record_ibfk_2` FOREIGN KEY (`stuid`) REFERENCES `qlms_user` (`stuid`) ON DELETE CASCADE ON UPDATE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
