-- phpMyAdmin SQL Dump
-- version 4.5.4.1deb2ubuntu2
-- http://www.phpmyadmin.net
--
-- Počítač: localhost
-- Vytvořeno: Pon 04. pro 2017, 22:44
-- Verze serveru: 5.7.20-0ubuntu0.16.04.1
-- Verze PHP: 7.1.12-1+ubuntu16.04.1+deb.sury.org+1

SET SQL_MODE = "NO_AUTO_VALUE_ON_ZERO";
SET time_zone = "+00:00";


/*!40101 SET @OLD_CHARACTER_SET_CLIENT=@@CHARACTER_SET_CLIENT */;
/*!40101 SET @OLD_CHARACTER_SET_RESULTS=@@CHARACTER_SET_RESULTS */;
/*!40101 SET @OLD_COLLATION_CONNECTION=@@COLLATION_CONNECTION */;
/*!40101 SET NAMES utf8mb4 */;

--
-- Databáze: `iisprojekt`
--

-- --------------------------------------------------------

--
-- Struktura tabulky `degrees`
--

CREATE TABLE `degrees` (
  `user_position_id` int(11) NOT NULL,
  `id_titul_pred` varchar(50) DEFAULT NULL,
  `id_titul_za` varchar(50) DEFAULT NULL,
  `created_at` date DEFAULT NULL,
  `updated_at` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `degrees`
--

INSERT INTO `degrees` (`user_position_id`, `id_titul_pred`, `id_titul_za`, `created_at`, `updated_at`) VALUES
(1, 'Ing.', NULL, NULL, NULL),
(2, NULL, NULL, NULL, NULL),
(3, 'Ing.', 'Ph.D, CSc.', NULL, NULL),
(4, NULL, NULL, NULL, NULL),
(5, NULL, NULL, NULL, NULL),
(6, NULL, NULL, NULL, NULL),
(7, NULL, NULL, NULL, NULL),
(8, NULL, NULL, NULL, NULL),
(9, 'Ing.', 'Ph.D, CSc.', NULL, NULL),
(10, 'Ing.', 'Ph.D', NULL, NULL),
(11, NULL, NULL, NULL, NULL),
(12, NULL, NULL, NULL, NULL),
(13, 'Ing.', 'PhDr.', NULL, NULL),
(14, NULL, NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Struktura tabulky `exams`
--

CREATE TABLE `exams` (
  `id` int(11) NOT NULL,
  `typ` varchar(64) DEFAULT NULL,
  `max_pocet_bodu` float DEFAULT NULL,
  `min_pocet_bodu_pro_slozeni` float DEFAULT NULL,
  `pocet_terminu` int(11) DEFAULT NULL,
  `max_pocet_terminu_pro_studenta` int(11) DEFAULT NULL,
  `subject_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `exams`
--

INSERT INTO `exams` (`id`, `typ`, `max_pocet_bodu`, `min_pocet_bodu_pro_slozeni`, `pocet_terminu`, `max_pocet_terminu_pro_studenta`, `subject_id`) VALUES
(1, 'Pulsemestralni zkouska', 20, 5, 2, 1, 1),
(2, 'Semestralni zkouska', 100, 0, 4, 2, 2),
(3, 'Pulsemestralni zkouska', 30, 0, 2, 1, 3),
(4, 'Semestralni zkouska', 70, 50, 2, 1, 3),
(5, 'Semestralni zkouska', 80, 20, 3, 3, 1),
(6, 'Pulsemestralni zkouska', 15, 0, 1, 1, 4);

-- --------------------------------------------------------

--
-- Struktura tabulky `migrations`
--

CREATE TABLE `migrations` (
  `id` int(10) UNSIGNED NOT NULL,
  `migration` varchar(255) COLLATE utf8mb4_unicode_ci NOT NULL,
  `batch` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8mb4 COLLATE=utf8mb4_unicode_ci;

--
-- Vypisuji data pro tabulku `migrations`
--

INSERT INTO `migrations` (`id`, `migration`, `batch`) VALUES
(1, '2014_10_12_000000_create_users_table', 1),
(2, '2014_10_12_100000_create_password_resets_table', 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `people`
--

CREATE TABLE `people` (
  `position_id` int(11) NOT NULL,
  `jmeno` varchar(120) CHARACTER SET utf8 COLLATE utf8_czech_ci NOT NULL,
  `prijmeni` varchar(120) CHARACTER SET utf8 COLLATE utf8_czech_ci NOT NULL,
  `datum_narozeni` date DEFAULT NULL,
  `rodne_cislo` varchar(60) CHARACTER SET utf8 COLLATE utf8_czech_ci DEFAULT NULL,
  `password` varchar(255) NOT NULL,
  `username` varchar(255) CHARACTER SET utf16 COLLATE utf16_czech_ci NOT NULL,
  `email` varchar(255) CHARACTER SET utf8 COLLATE utf8_czech_ci DEFAULT NULL,
  `position_type` char(15) DEFAULT NULL,
  `remember_token` char(100) DEFAULT NULL,
  `updated_at` date DEFAULT NULL,
  `created_at` date DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `people`
--

INSERT INTO `people` (`position_id`, `jmeno`, `prijmeni`, `datum_narozeni`, `rodne_cislo`, `password`, `username`, `email`, `position_type`, `remember_token`, `updated_at`, `created_at`) VALUES
(1, 'Petr', 'Dvořák', '1950-07-08', '500708019', '$2a$06$fCMc8CKD6BaKq9wuibAE9eL5Cijb7Apjd/VfCuUleumvQMkmMYGl2', 'xdvora00', 'xdvora00@is.cz', 'App\\teacher', 'aotZo42IRPIvZaEbtHxLTzqp9eAJCThGobSKkKzxpibIUJ0SBRlzL4wiFQ6G', NULL, NULL),
(2, 'Jiri', 'Novák', '1996-08-08', '9608080020', '$2a$06$fXuaM6btFHsu.Qq3Ts2eU.g17D86Dx6S5F4CqahR/Vo1VuN.OL/j2', 'xnovak10', 'xnovak10@is.cz', 'App\\student', '62brHZ90MHEqf6C4TJrVCTqdkrXGG4mex0e9rtm66VDrOkm2Wg3O51qBURr2', NULL, NULL),
(3, 'Ondra', 'Valach', '1942-04-10', '420410005', '$2a$06$VJOH7cxyHvJqwi4cR8FjxezrmzJAJII9Qo.4R0cBgxzwvqFq6DzSC', 'xvalac09', 'xvalac09@is.cz', 'App\\teacher', 'wesdkJXKDrNSPmOXweqX8g3XXCw93lQWPpqvTMN0ezjMmFBA5IFHR9L305nm', NULL, NULL),
(4, 'Michal', 'Hodes', '1997-10-21', '9709210104', '$2a$06$jx0QzaakNcOlPfOA3aNVMO7I4Qchf5CMWo5rbKZkfK0tYBL4C7Sjm', 'xhodes02', 'xhodes02@is.cz', 'App\\student', NULL, NULL, NULL),
(5, 'Tomáš', 'Holík', '1995-09-21', '9509210106', '$2a$06$J1pkN8YyYM0I2A9ivqPjc.jkYtooOtH4iV77UtZ6RC5vIjX3.NFqm', 'xholik13', 'xholik13@is.cz', 'App\\student', NULL, NULL, NULL),
(6, 'Norbert', 'Květena', '1995-09-28', '9509284928', '$2a$06$PyuXSwzSaSwbXM5Idpx/A.M3DRMmQC56BsgW14VHondN1SH7jCgD6', 'xkvete14', 'xkvete14@is.cz', 'App\\student', NULL, NULL, NULL),
(7, 'Thai', 'Ho Xuan', '1991-08-07', '9108070202', '$2a$06$oDdpy8F1LoDUB1H.qI0.Mu/3b3VsMESWJgvSWBffiY1G80ZNa0LfG', 'xhoxua00', 'xhoxua00@is.cz', 'App\\student', NULL, NULL, NULL),
(8, 'Olaf', 'Tryggvason', '1990-09-15', '9009150106', '$2a$06$fuaEqMf/iqMXBSkWX0xo1OI0pt6w8JsDk0EEQbVsqGWbjRaMGcX3O', 'xtrygg01', 'xtrygg01@is.cz', 'App\\student', NULL, NULL, NULL),
(9, 'Petr', 'Němec', '1953-05-05', '530505005', '$2a$06$J982NDovSfC3JjNxYm9ahOt0bW9iuk3x02kqKTetu6PTGDApl7LRO', 'xnemec42', 'xnemec42@is.cz', 'App\\teacher', NULL, NULL, NULL),
(10, 'Albert', 'Krček', '1952-03-24', '520324099', '$2a$06$4MnwiP/ortHCeNCLrKA7H.TqhiDSkhMv1wRUOSKteL.WkuhbmuYY6', 'xkrcek09', 'xkrcek09@is.cz', 'App\\teacher', NULL, NULL, NULL),
(11, 'Vladimíra', 'Okvětová', '1992-04-15', '9204151000', '$2a$06$VmfBwD5sv5IM7yyX.3tEYeZG8tJ0/Cnl1pjAANU4SAOqX4Kma2Z3K', 'xokvet05', 'xokvet05@is.cz', 'App\\student', NULL, NULL, NULL),
(12, 'Luboš', 'Masopud', '1996-12-24', '9612241001', '$2a$06$Ps9MwG142OxGcYMjrqJV4.Is.ZWpa6u8VqCRW2weoEbXS33X7Ik0C', 'xmasop03', 'xmasop03@is.cz', 'App\\student', NULL, NULL, NULL),
(13, 'Zdeněk', 'Červinka', '2000-03-01', '55555555555', '$2a$06$KpFf8nBNyBI0ZyVe/hg/outi2.Ftgl.9LdduO4xcJ8Bo6OdSLXlmu', 'xcervi00', 'xcervi00@is.cz', 'App\\teacher', 'y9FQm6c3RPqwR1vqsP7zv4dzt9wVAS9lO3hpQLMelWL29hJOLsMG8ZOYDf8I', NULL, NULL),
(14, 'Petr', 'Buchal', '2000-12-07', '9602078648', '$2a$06$4gclYvXtu3ncjL8rzHiNWOhqn6SQjkSkHn2HldwrxEb9O3vewzy4y', 'xbucha02', 'xbucha02@is.cz', 'App\\student', 'lN5b2TsiagdBHp8klqOVKjo48a9GNKygce25YkdNr0B4s59tG3mTyxUNlLrm', NULL, NULL),
(15, 'Josef', 'Kováčik', '1973-09-03', '7303097812', '$2a$06$s3hGApsIh3NklM61JDT8/u2rHPqBYllyVoElx.K.PzxGF4IscxNCC', 'xkovac08', 'xkovac08@is.cz', 'App\\student', NULL, NULL, NULL);

-- --------------------------------------------------------

--
-- Struktura tabulky `questions`
--

CREATE TABLE `questions` (
  `id` int(11) NOT NULL,
  `typ_otazky` int(11) DEFAULT NULL,
  `zadani_otazky` varchar(1024) DEFAULT NULL,
  `max_pocet_bodu` float DEFAULT NULL,
  `min_pocet_bodu_pro_slozeni` float DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `questions`
--

INSERT INTO `questions` (`id`, `typ_otazky`, `zadani_otazky`, `max_pocet_bodu`, `min_pocet_bodu_pro_slozeni`) VALUES
(3, 1, 'Spocitej 18/9', 5, 0),
(4, 1, 'Spocitej 18/3', 5, 0),
(5, 1, 'Spocitej 3!', 10, 0),
(6, 1, 'Proved normalizaci', 80, 0),
(7, 1, 'Co je deadlock?', 90, 0),
(8, 1, 'Nakresli ERD', 85, 0),
(9, 1, 'Nakresli PN', 70, 0),
(10, 1, 'Co je kruhova konvoluce', 95, 0),
(20, 1, 'Spocitej 4! / 3', 5, 0);

-- --------------------------------------------------------

--
-- Struktura tabulky `question_term`
--

CREATE TABLE `question_term` (
  `question_id` int(11) NOT NULL,
  `term_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `question_term`
--

INSERT INTO `question_term` (`question_id`, `term_id`) VALUES
(3, 1),
(4, 1),
(5, 1),
(7, 2),
(3, 5),
(4, 5),
(5, 5),
(5, 10),
(20, 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `scores`
--

CREATE TABLE `scores` (
  `id` int(11) NOT NULL,
  `pocet_bodu` float NOT NULL,
  `procentualni_uspesnost` float DEFAULT NULL,
  `komentar` char(200) CHARACTER SET cp1250 COLLATE cp1250_czech_cs DEFAULT NULL,
  `teacher_id` int(11) NOT NULL,
  `student_id` int(11) NOT NULL,
  `question_id` int(11) DEFAULT NULL,
  `term_id` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `scores`
--

INSERT INTO `scores` (`id`, `pocet_bodu`, `procentualni_uspesnost`, `komentar`, `teacher_id`, `student_id`, `question_id`, `term_id`) VALUES
(81, 3, 60, NULL, 1, 14, 3, 1),
(82, 2, 40, NULL, 1, 14, 4, 1),
(83, 4, 40, NULL, 1, 14, 5, 1),
(84, 5, 100, 'Dobrá práce!', 1, 14, 20, 1),
(85, 1, 20, NULL, 1, 8, 3, 1),
(86, 0, 0, NULL, 1, 8, 4, 1),
(87, 5, 50, NULL, 1, 8, 5, 1),
(88, 4, 80, NULL, 1, 8, 20, 1),
(89, 7, 140, NULL, 1, 7, 3, 1),
(90, 2, 40, NULL, 1, 7, 4, 1),
(91, 3, 30, NULL, 1, 7, 5, 1),
(92, 5, 100, NULL, 1, 7, 20, 1),
(93, 5, 100, NULL, 1, 6, 3, 1),
(94, 0, 0, NULL, 1, 6, 4, 1),
(95, 4, 40, NULL, 1, 6, 5, 1),
(96, 2, 40, NULL, 1, 6, 20, 1),
(97, 4, 80, NULL, 1, 5, 3, 1),
(98, 5, 100, NULL, 1, 5, 4, 1),
(99, 5, 50, NULL, 1, 5, 5, 1),
(100, 3, 60, NULL, 1, 5, 20, 1),
(101, 2, 40, NULL, 1, 2, 3, 1),
(102, 5, 100, NULL, 1, 2, 4, 1),
(103, 3, 30, NULL, 1, 2, 5, 1),
(104, 5, 100, NULL, 1, 2, 20, 1),
(105, 50, 55.5556, NULL, 1, 2, 7, 2),
(106, 85, 94.4444, NULL, 1, 14, 7, 2),
(107, 70, 77.7778, NULL, 1, 7, 7, 2),
(108, 60, 66.6667, NULL, 1, 6, 7, 2),
(109, 80, 88.8889, NULL, 1, 8, 7, 2),
(110, 0, 0, NULL, 1, 12, 7, 2);

-- --------------------------------------------------------

--
-- Struktura tabulky `students`
--

CREATE TABLE `students` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `students`
--

INSERT INTO `students` (`id`) VALUES
(2),
(4),
(5),
(6),
(7),
(8),
(11),
(12),
(14),
(15);

-- --------------------------------------------------------

--
-- Struktura tabulky `student_subject`
--

CREATE TABLE `student_subject` (
  `student_id` int(11) NOT NULL,
  `subject_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `student_subject`
--

INSERT INTO `student_subject` (`student_id`, `subject_id`) VALUES
(2, 1),
(2, 2),
(6, 2),
(8, 2),
(12, 2),
(8, 1),
(2, 3),
(4, 3),
(4, 4),
(4, 5),
(5, 4),
(5, 5),
(5, 1),
(6, 5),
(6, 1),
(7, 1),
(7, 2),
(7, 3),
(8, 3),
(11, 3),
(11, 4),
(11, 5),
(12, 4),
(12, 5),
(14, 4),
(14, 1),
(14, 2),
(15, 3),
(15, 4),
(15, 5);

-- --------------------------------------------------------

--
-- Struktura tabulky `student_term`
--

CREATE TABLE `student_term` (
  `student_id` int(11) DEFAULT NULL,
  `term_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `student_term`
--

INSERT INTO `student_term` (`student_id`, `term_id`) VALUES
(14, 1),
(8, 1),
(7, 1),
(6, 1),
(5, 1),
(2, 2),
(14, 2),
(7, 2),
(6, 2),
(8, 2),
(12, 2),
(15, 5),
(4, 5),
(2, 5),
(7, 5),
(8, 5),
(11, 5),
(2, 1);

-- --------------------------------------------------------

--
-- Struktura tabulky `subjects`
--

CREATE TABLE `subjects` (
  `zkratka` varchar(20) NOT NULL,
  `pocet_kreditu` int(11) NOT NULL,
  `garant_id` int(11) NOT NULL,
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `subjects`
--

INSERT INTO `subjects` (`zkratka`, `pocet_kreditu`, `garant_id`, `id`) VALUES
('IIS', 5, 3, 1),
('ITU', 5, 1, 2),
('ISA', 5, 9, 3),
('IMP', 6, 13, 4),
('IMS', 5, 10, 5);

-- --------------------------------------------------------

--
-- Struktura tabulky `subject_teacher`
--

CREATE TABLE `subject_teacher` (
  `teacher_id` int(11) NOT NULL,
  `subject_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `subject_teacher`
--

INSERT INTO `subject_teacher` (`teacher_id`, `subject_id`) VALUES
(1, 1),
(1, 2),
(9, 1),
(10, 2),
(3, 1),
(1, 3),
(9, 3),
(13, 4),
(10, 5),
(10, 3),
(13, 5),
(13, 2),
(3, 4),
(3, 5),
(9, 4);

-- --------------------------------------------------------

--
-- Struktura tabulky `teachers`
--

CREATE TABLE `teachers` (
  `id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `teachers`
--

INSERT INTO `teachers` (`id`) VALUES
(1),
(3),
(9),
(10),
(13);

-- --------------------------------------------------------

--
-- Struktura tabulky `terms`
--

CREATE TABLE `terms` (
  `id` int(11) NOT NULL,
  `datum_cas` char(50) DEFAULT NULL,
  `misto` varchar(90) DEFAULT NULL,
  `zacatek_prihlasovani` date DEFAULT NULL,
  `konec_prihlasovani` date DEFAULT NULL,
  `max_pocet_studentu` int(11) DEFAULT NULL,
  `cislo_terminu` int(11) NOT NULL,
  `exam_id` int(11) NOT NULL
) ENGINE=InnoDB DEFAULT CHARSET=latin1;

--
-- Vypisuji data pro tabulku `terms`
--

INSERT INTO `terms` (`id`, `datum_cas`, `misto`, `zacatek_prihlasovani`, `konec_prihlasovani`, `max_pocet_studentu`, `cislo_terminu`, `exam_id`) VALUES
(1, '2017-12-24 22:00:00', 'E112', '2017-12-01', '2017-12-20', 6, 1, 1),
(2, '2017-12-25 22:00:00', 'E112', '2017-12-01', '2017-12-15', 10, 1, 2),
(4, '2018-01-03 14:00:00', 'D105', '2017-12-25', '2017-01-30', 10, 2, 2),
(5, '2017-12-01 00:00:00', 'D105', '2017-11-08', '2017-11-15', 10, 1, 3),
(6, '2017-12-22 22:00:00', 'D105', '2017-12-02', '2017-12-12', 10, 2, 3),
(7, '2018-01-06 14:00:00', 'D105', '2017-12-01', '2017-12-31', 10, 1, 4),
(8, '2018-01-12 14:00:00', 'D105', '2017-12-01', '2017-12-31', 10, 2, 4),
(9, '2018-01-20 12:00:00', 'E112', '2017-12-01', '2017-12-16', 5, 1, 5),
(10, '2017-12-28 14:00:00', 'D105', '2017-12-01', '2017-12-07', 10, 1, 6);

--
-- Klíče pro exportované tabulky
--

--
-- Klíče pro tabulku `degrees`
--
ALTER TABLE `degrees`
  ADD PRIMARY KEY (`user_position_id`);

--
-- Klíče pro tabulku `exams`
--
ALTER TABLE `exams`
  ADD PRIMARY KEY (`id`),
  ADD KEY `con_exam_predmet` (`subject_id`);

--
-- Klíče pro tabulku `migrations`
--
ALTER TABLE `migrations`
  ADD PRIMARY KEY (`id`);

--
-- Klíče pro tabulku `people`
--
ALTER TABLE `people`
  ADD PRIMARY KEY (`position_id`),
  ADD UNIQUE KEY `email` (`email`);

--
-- Klíče pro tabulku `questions`
--
ALTER TABLE `questions`
  ADD PRIMARY KEY (`id`);

--
-- Klíče pro tabulku `question_term`
--
ALTER TABLE `question_term`
  ADD KEY `con_cislo_otazky` (`question_id`),
  ADD KEY `con_kod_terminu` (`term_id`);

--
-- Klíče pro tabulku `scores`
--
ALTER TABLE `scores`
  ADD PRIMARY KEY (`id`),
  ADD KEY `con_udeluje` (`teacher_id`),
  ADD KEY `con_jehodnocen` (`student_id`),
  ADD KEY `score_id` (`question_id`) USING BTREE,
  ADD KEY `term_id` (`term_id`);

--
-- Klíče pro tabulku `students`
--
ALTER TABLE `students`
  ADD PRIMARY KEY (`id`);

--
-- Klíče pro tabulku `student_subject`
--
ALTER TABLE `student_subject`
  ADD KEY `con_student_v_predmetu` (`student_id`),
  ADD KEY `con_predmet_studenti` (`subject_id`);

--
-- Klíče pro tabulku `student_term`
--
ALTER TABLE `student_term`
  ADD KEY `con_prihlaseni_studenti` (`student_id`),
  ADD KEY `con_na_termin` (`term_id`);

--
-- Klíče pro tabulku `subjects`
--
ALTER TABLE `subjects`
  ADD PRIMARY KEY (`id`),
  ADD UNIQUE KEY `id` (`id`),
  ADD UNIQUE KEY `id2` (`zkratka`),
  ADD KEY `con_garant` (`garant_id`);

--
-- Klíče pro tabulku `subject_teacher`
--
ALTER TABLE `subject_teacher`
  ADD KEY `con_vyucujici` (`teacher_id`),
  ADD KEY `con_predmet_vyucujici` (`subject_id`);

--
-- Klíče pro tabulku `teachers`
--
ALTER TABLE `teachers`
  ADD PRIMARY KEY (`id`);

--
-- Klíče pro tabulku `terms`
--
ALTER TABLE `terms`
  ADD PRIMARY KEY (`id`),
  ADD KEY `con_kod_zkousky` (`exam_id`);

--
-- AUTO_INCREMENT pro tabulky
--

--
-- AUTO_INCREMENT pro tabulku `exams`
--
ALTER TABLE `exams`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=10;
--
-- AUTO_INCREMENT pro tabulku `migrations`
--
ALTER TABLE `migrations`
  MODIFY `id` int(10) UNSIGNED NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=3;
--
-- AUTO_INCREMENT pro tabulku `people`
--
ALTER TABLE `people`
  MODIFY `position_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=38;
--
-- AUTO_INCREMENT pro tabulku `questions`
--
ALTER TABLE `questions`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;
--
-- AUTO_INCREMENT pro tabulku `question_term`
--
ALTER TABLE `question_term`
  MODIFY `question_id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=22;
--
-- AUTO_INCREMENT pro tabulku `scores`
--
ALTER TABLE `scores`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=111;
--
-- AUTO_INCREMENT pro tabulku `terms`
--
ALTER TABLE `terms`
  MODIFY `id` int(11) NOT NULL AUTO_INCREMENT, AUTO_INCREMENT=16;
--
-- Omezení pro exportované tabulky
--

--
-- Omezení pro tabulku `degrees`
--
ALTER TABLE `degrees`
  ADD CONSTRAINT `con_osoba_s_titulem` FOREIGN KEY (`user_position_id`) REFERENCES `people` (`position_id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `exams`
--
ALTER TABLE `exams`
  ADD CONSTRAINT `con_exam_predmet` FOREIGN KEY (`subject_id`) REFERENCES `subjects` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `question_term`
--
ALTER TABLE `question_term`
  ADD CONSTRAINT `con_cislo_otazky` FOREIGN KEY (`question_id`) REFERENCES `questions` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `con_kod_terminu` FOREIGN KEY (`term_id`) REFERENCES `terms` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `scores`
--
ALTER TABLE `scores`
  ADD CONSTRAINT `con_jehodnocen` FOREIGN KEY (`student_id`) REFERENCES `students` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `con_question` FOREIGN KEY (`question_id`) REFERENCES `questions` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `con_termin` FOREIGN KEY (`term_id`) REFERENCES `terms` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `con_udeluje` FOREIGN KEY (`teacher_id`) REFERENCES `teachers` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `students`
--
ALTER TABLE `students`
  ADD CONSTRAINT `con_osoba_student` FOREIGN KEY (`id`) REFERENCES `people` (`position_id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `student_subject`
--
ALTER TABLE `student_subject`
  ADD CONSTRAINT `con_predmet_studenti` FOREIGN KEY (`subject_id`) REFERENCES `subjects` (`id`),
  ADD CONSTRAINT `con_student_v_predmetu` FOREIGN KEY (`student_id`) REFERENCES `students` (`id`);

--
-- Omezení pro tabulku `student_term`
--
ALTER TABLE `student_term`
  ADD CONSTRAINT `con_na_termin` FOREIGN KEY (`term_id`) REFERENCES `terms` (`id`) ON DELETE CASCADE,
  ADD CONSTRAINT `con_prihlaseni_studenti` FOREIGN KEY (`student_id`) REFERENCES `students` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `subjects`
--
ALTER TABLE `subjects`
  ADD CONSTRAINT `con_garant` FOREIGN KEY (`garant_id`) REFERENCES `teachers` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `subject_teacher`
--
ALTER TABLE `subject_teacher`
  ADD CONSTRAINT `con_predmet_vyucujici` FOREIGN KEY (`subject_id`) REFERENCES `subjects` (`id`),
  ADD CONSTRAINT `con_vyucujici` FOREIGN KEY (`teacher_id`) REFERENCES `teachers` (`id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `teachers`
--
ALTER TABLE `teachers`
  ADD CONSTRAINT `con_osoba_vyucujici` FOREIGN KEY (`id`) REFERENCES `people` (`position_id`) ON DELETE CASCADE;

--
-- Omezení pro tabulku `terms`
--
ALTER TABLE `terms`
  ADD CONSTRAINT `con_kod_zkousky` FOREIGN KEY (`exam_id`) REFERENCES `exams` (`id`) ON DELETE CASCADE;

/*!40101 SET CHARACTER_SET_CLIENT=@OLD_CHARACTER_SET_CLIENT */;
/*!40101 SET CHARACTER_SET_RESULTS=@OLD_CHARACTER_SET_RESULTS */;
/*!40101 SET COLLATION_CONNECTION=@OLD_COLLATION_CONNECTION */;
