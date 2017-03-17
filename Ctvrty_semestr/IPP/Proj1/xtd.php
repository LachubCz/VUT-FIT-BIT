<?php
/**
 * @author Petr Buchal(xbucha02)
 */

####################################################################################
#############################Tridy pro ukladani hodnot##############################
####################################################################################

/**
 * trida na ulozeni pole tabulek
 */
class database
{
	var $arrayoftables;  //pole tabulek
	var $arguments;  //pole argumentu skriptu

	/**
	 * [metoda inicializuje pole v instanci tridy database]
	 * @param  [array] $types [argumenty skriptu]
	 * @return [void]
	 */
	function init($types)
	{
		$this->arrayoftables = array();
		$this->arguments = $types;
	}

	/**
	 * [metoda pridava tabulku do arrayoftables]
	 * @param  [table] $table [tabulka]
	 * @return [void]
	 */
	function addtable ($table)
	{
		$this->arrayoftables[$table->name] = $table;
	}
}

/**
 * trida na ulozeni jednotlivych tabulek 
 */
class table
{
	var $name;  //jmeno tabulky
	var $primarykeys;  //pole jmen podelementu
	var $attributes;  //pole atributu

	/**
	 * [metoda inicializuje pole a nastavi jmeno instance tridy]
	 * @param [string] $name [jmeno tabulky]
	 */
	function set_name($name)
	{
		$this->name = $name;
		$this->primarykeys = array();
		$this->attributes = array();
	}
}

####################################################################################
###########################Samotne telo mocneho programu############################
####################################################################################

//vytvoreni pole do ktereho se ukladaji hodnoty argumentu
$types = array("2"=>"-1", "3"=>"-1", "4"=>"-1", "5"=>"-1", "6"=>"-1", "7"=>"-1", "8"=>"-1", );

//cyklus, ktery zpracuje argumenty pomoci jednotlivych funkci na zpracovani argumentu (help_print, help_test, parameter_test, parameter_value) a pomocne funkce (isfloat)
for ($i=1; $i < $argc; $i++) 
{ 
	if ($argc === 1) 
		break;

	if ($argc === 2) 
	{
		if (!(strcmp ($argv[$i], "--help")))
		{
		  	help_print();
			exit(0);
		}
	}
	else
		help_test($argv, $argc);

	if ($argc > 7) 
		exit(1);

	$type = parameter_test($argv[$i]);

	if ($types[$type] === '-1') 
		$types[$type] = parameter_value($type, $argv[$i]);
	else
		exit(1);
}

//pokud je zaroven zadan argument skriptu --etc a -b, skript konci s navratovou hodnotou 1
if ($types['5'] !== '-1')
{	
	if ($types['7'] === 1) 
	{
		exit(1);
	}
}

//vytvori se nova databaze
$database = new database();

//pomoci funkce (fileload) se nacte a rozparseruje vstupni soubor, jako pomocna funkce pro zjisteni existence nebo neexistence souboru slouzi funkce (emptyfile)
$file = fileload($types['2'], $types['3']);

//inicializuji se pole databaze a do promene arguments se vlozi seznam zadanych argumentu skriptu
$database->init($types);

//rekurzivne pomoci funkce (recursivegold) a pomocnych funkci (uelements, arraytodb, prkuniq, prkedit, control, children_names) projde obsah souboru na vstupu a v hrube podobe se ulozi do databaze
$database = recursivegold ($file, $database);

//case_insensitive_cmp odstrani chyby vznikle case sensitive ukladanim nekterych dat ve funkcich recursivegold a uelements
$database = case_insensitive_cmp($database);

$database = case_insentive_db_edit($database);

//print_r($database);

//databaze se pozmeni do finalni formy podle argumentu --etc a -b
$database = etc_b_correction($database);

//kontrola zda-li neni shoda ve jmene atributu a jmene podelementu
attcheck ($database);

//pokud byl zadan parametr -g tiskne se XML soubor, pokud zadan nebyl tisknou se SQL prikazy
if ($database->arguments['8'] === '-1') 
{
	print_database($database);
}
else
{
	$GLOBALS['array'] = array();  //globalni promena vytvorena pro zaznamenani jiz vytisknutych vztahu mezi funkcemi
	xml_print($database);
}

####################################################################################
######################Funkce pro zpracovani argumentu skriptu#######################
####################################################################################

/**
 * [tisk cele napovedy]
 * @return [void]
 */
function help_print()
{
	echo "--help viz společné zadání všech úloh"; echo"\n";
	echo "--input=filename zadaný vstupní soubor ve formátu XML"; echo"\n";
	echo "--output=filename zadaný výstupní soubor ve formátu definovaném výše"; echo"\n";
	echo "--header='hlavička' na začátek výstupního souboru se vloží zakomentovaná hlavička"; echo"\n";
	echo "--etc=n pro n ≥ 0 určuje maximální počet sloupců vzniklých ze stejnojmenných podelementů"; echo"\n";
	echo "-a nebudou se generovat sloupce z atributů ve vstupním XML souboru"; echo"\n";
	echo "-b pokud bude element obsahovat více podelementů stejného názvu, bude se uvažovat, jako by zde byl pouze jediný takový (tento parametr nesmí být kombinován s parametrem --etc=n)"; echo"\n";
 	echo "-g lze jej uplatnit v kombinaci s jakýmikoliv jinými přepínači vyjma --help. Při jeho aktivaci bude výstupním souborem pouze XML"; echo"\n";
}

/**
 * [pokud je nektery z argumentu "--help" a argumentu je vic nez dva, ukoncuje program "exit(1)"]
 * @param  [array] $argv [pole argumentu]
 * @param  [int] $argc [pocet argumentu]
 * @return [void]
 */
function help_test($argv, $argc)
{
	$index = 1;

	while ($index < $argc) 
	{
		if (!(strcmp ($argv[$index], "--help")))
		{
			exit(1);
		}

		$index++;
	}
}

/**
 * [vraci hodnotu parametru, pokud je parametr jiny nez v zadani ukoncuje program "exit(1)"]
 * @param  [string] $parameter [parametr]
 * @return [int]            [cislo, ktere pozdeji parametr reprezentuje]
 */
function parameter_test($parameter)
{
	if (strlen($parameter) === 2)
	{
		if (!(strcmp ($parameter, "-a")))
			return 6;

		if (!(strcmp ($parameter, "-b")))
			return 7;

		if (!(strcmp ($parameter, "-g")))
			return 8;
	}

	if (!(strlen($parameter) >= 6))
	{
		exit(1);
	}
	else
	{
		$cropped = substr($parameter, 0, 6);

		if (!(strcmp ($cropped, "--etc=")))
			return 5;
	}

	if (!(strlen($parameter) >= 8))
	{
		exit(1);
	}
	else
	{
		$cropped = substr($parameter, 0, 8);

		if (!(strcmp ($cropped, "--input=")))
			return 2;
	}

	if (!(strlen($parameter) >= 9))
	{
		exit(1);
	}
	else
	{
		$cropped = substr($parameter, 0, 9);

		if (!(strcmp ($cropped, "--header=")))
			return 4;

		if (!(strcmp ($cropped, "--output=")))
			return 3;		
	}

	exit(1);
}

/**
 * [vraci hodnoty parametru skriptu, pokud je hodnota nespravna program konci "exit(1)"]
 * @param  [int] $type      [ciselny typ parametru]
 * @param  [string] $parameter [parametr]
 * @return [string]            [hodnota parametru]
 */
function parameter_value($type, $parameter)
{	
	switch(true)
	{
		case ($type === 2):
			parse_str($parameter, $output);
			$value = $output['--input'];
			break;

		case ($type === 3):
			parse_str($parameter, $output);
			$value = $output['--output'];
			break;

		case ($type === 4):
			parse_str($parameter, $output);
			$value = $output['--header'];
			break;

		case ($type === 5):
			parse_str($parameter, $output);
			$value = $output['--etc'];

			if (!(is_numeric($value)) || (isfloat($value)))
			{
				exit(1);
			}
			break;

		default:
			return 1;
	}

	return $value;
}

####################################################################################
######################Funkce pro zpracovani vstupniho souboru#######################
####################################################################################

/**
 * [kontrola prazdnosti souboru a vstupu STDIN + kontrola existence souboru]
 * @param  [string] $name   [jmeno souboru, popripade stringu]
 * @param  [int] $type   [mod, ktery urcuje, zda-li se jedna o string nebo o soubor]
 * @param  [string] $output [destinace, kam by se popripade tiskl prazdny soubor]
 * @return [void]
 */
function emptyfile($name, $type, $output)
{
	if ($type === '1') 
	{
		$xml = simplexml_load_string($name);

	 	if ($xml->count() === 0) 
	 	{
	 		output ("", $output);
	 	}
	}
	else
	{
		if (file_exists($name)) 
		{
			if (filesize($name) === 0)
			{
				output ("", $output);
			}

		    $xml = simplexml_load_file($name);

		 	if ($xml->count() === 0)
		 	{
		 		output ("", $output);
		 	}
		}
		else
		{
			exit(2);
		}
	}
}

/**
 * [nacteni a rozparserovani souboru, pokud neni soubor zadan nacteni dat ze STDIN a jejich rozparserovani]
 * @param  [string] $file   [hodnota argumentu --input]
 * @param  [string] $output [destinace, kam by se popripade tiskl prazdny soubor]
 * @return [SimpleXMLElement]         [funkce vraci rozparserovany soubor, popripade string]
 */
function fileload($file, $output)
{
	if ($file === '-1') 
	{
		$file = "";
		$f = fopen( 'php://stdin', 'r' );

		while( $line = fgets($f)) 
		{
		  $file = $file . $line;
		}

		fclose($f);

		emptyfile($file, '1', $output);
		$file = simplexml_load_string($file);
	}
	else
	{
		emptyfile($file, '0', $output);
		$file = simplexml_load_file($file);
	}

	return $file;
}

####################################################################################
########################Funkce pro ukladani XML do databaze#########################
####################################################################################

/**
 * [funkce vrati pole jmen podelemetu daneho SimpleXMLElementu]
 * @param  [SimpleXMLElement] $file [SimpleXMLElement]
 * @return [array]       [pole jmen podelementu]
 */
function children_names ($file)
{
	$namescount = $file->count();
	$namesarr = array();
	
	foreach ($file->children() as $child)
	{
		$name = $child->getName();
		array_push($namesarr, $name);
	}

	return $namesarr;
}

/**
 * [funkce urci jakeho datoveho typu je hodnota predana v parametru $val]
 * @param  [string] $val  [$string u jehoz obsahu se zjistuje datovy typ]
 * @param  [int] $type [mod, zdali se jedna o atribut nebo o klic]
 * @return [string]       [hledany datovy typ]
 */
function control($val, $type)
{
	if (empty($val) || ctype_space($val) || $val == "0" || $val == "1" || !strcasecmp($val, "true")|| !strcasecmp($val, "false")) 
	{
		$val = "BIT";
	}
	else if (isfloat("$val")) 
		{
			$val = "FLOAT";
		}
	else if (is_numeric("$val")) 
		{
			$val = "INT";
		}
	else if ($type === '1') 
		{
			$val = "NVARCHAR";
		}
	else
	{
		$val = "NTEXT";
	}

	return $val;
}

/**
 * [funkce odstranuje prvky typu "item_id" pokud je v poli zaroven i prvek "item1_id"]
 * @param  [array] $arrdata [stare pole prvku]
 * @param  [array] $arrnew  [nove pole prvku]
 * @return [array]          [mergenute pole ochuzene o nechtene prvky]
 */
function prkedit($arrdata, $arrnew)
{
	$arrdata = array_merge($arrdata, $arrnew);
	$allKeys = array_keys($arrdata);
	
	for ($i=0; $i < count($allKeys); $i++) 
	{ 
		$substing = substr($allKeys[$i], -4);

		if ($substing === "1_id") 
		{
			$len = strlen($allKeys[$i]) - 4;
			$delstr = substr($allKeys[$i], 0, $len);
			$delstr .= "_id";
			
			if (array_key_exists($delstr, $arrdata)) 
			{
				unset($arrdata[$delstr]);
			}
		}
	}

	return $arrdata;
}

/**
 * [vklada do pole novy prvek, pokud prvek daneho nazvu existuje, vsem prvkum priradi odpovidajici indexy]
 * @param  [array] $array [pole prvku]
 * @param  [string] $name  [jmeno nove vkladaneho prvku]
 * @param  [string] $type  [datovy typ vkladaneho prku]
 * @return [array]        [pole obohacene o vladany prvek]
 */
function prkuniq($array, $name, $type)
{
	$num = 1;
	$namenumID = $name . $num . "_id";
	$nameID = $name . "_id";

	if (array_key_exists($nameID, $array))
	{
		$temp = $array[$nameID];
		unset($array[$nameID]);
		$array[$namenumID] = $temp;
		$num+=1;
		$namenumID = $name . $num . "_id";
		$array[$namenumID] = $type;
	}
	else
	{
		if (!(array_key_exists($namenumID, $array))) 
		{
			$array[$nameID] = $type;
		}
		else
		{
			while (1) 
			{
				$num+=1;
				$namenumID = $name . $num . "_id";

				if (!(array_key_exists($namenumID, $array))) 
				{
					$array[$namenumID] = $type;
					break;
				}
			}
		}
	}

	return $array;
}

/**
 * [funkce vklada do tabulky v databazi aktualizovane pole primarykeys]
 * @param  [string] $name     [jmeno tabulky]
 * @param  [array] $arrayprk [pole primarykeys ziskane z posledniho zkoumaneho prvku]
 * @param  [database] $database [databaze pred aktualizaci]
 * @return [database]           [aktualizovana databaze]
 */
function arraytodb($name, $arrayprk, $database)
{
	$array = $database->arrayoftables;

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{ 
		$val = array_values($array)[$i];

		if ($val->name === $name) 
		{
			$primarykeys = prkedit($val->primarykeys, $arrayprk);
			$database->arrayoftables[$name]->primarykeys = $primarykeys;
		}	
	}

	return $database;
}

/**
 * [prochazi SimpleXMLElement a pro kazdou tabulku v databazi zkouma, zdali neobsahuje element s jeho jmenem, pokud ano, dochazi k aktualizaci hodnot v databazi pro danou tabulku]
 * @param  [database] $database [aktualni databaze]
 * @param  [SimpleXMLElement] $file     [SimpleXMLElement]
 * @return [database]           [obohacena databaze]
 */
function uelements ($database, $file)
{
	$countf = count($file);
	$countd = count($database->arrayoftables);
	
	for ($e=0; $e < $countd; $e++)
	{
		$array = array();
		$array = $database->arrayoftables;
		$val = array_values($array)[$e];

		for ($i=0; $i < count($file->{$val->name}); $i++)
		{
			$array = array();

			if (!empty($file->{$val->name}[$i]->__toString()) && !ctype_space($file->{$val->name}[$i]->__toString()))
			{
				$value_t = control($file->{$val->name}[$i], '0');
				$value_t = typeenum ($value_t, $database->arrayoftables[$val->name]->primarykeys["value"]);
				$database->arrayoftables[$val->name]->primarykeys["value"] = $value_t;
			}

			if ($database->arguments['6'] === '-1') 
			{
				foreach($file->{$val->name}[$i]->attributes() as $a => $b)
				{
					$type = control($b, '1');
					$a =  mb_strtolower ($a, 'UTF-8');

					if (array_key_exists($a, $database->arrayoftables[$val->name]->attributes)) 
					{
						$type = typeenum ($type, $database->arrayoftables[$val->name]->attributes[$a]);
					}

					$database->arrayoftables[$val->name]->attributes[$a] = $type;
				}
			}

			foreach ($file->{$val->name}[$i]->children() as $childreen)
			{
				$name = $childreen->getName();
				$name =  mb_strtolower ($name, 'UTF-8');

				if (empty($childreen))
				{
					$type = 0;
				}
				else
				{
					$type = control($childreen, '0');
				}

				$array = prkuniq($array, $name, $type);
			}

			$database = arraytodb($val->name, $array, $database);
		}
	}

	return $database;
}

/**
 * [rekurzivne prochazi SimpleXMLElement, zjistuje jmena podelementu (pomoci children_names), vytvari pro ne tabulky a nasledne do nich nahrava data (pomoci uelements), vse se uklada do database]
 * @param  [SimpleXMLElement] $file     [SimpleXMLElement]
 * @param  [database] $database [aktualni databaze]
 * @return [database]           [databaze]
 */
function recursivegold ($file, $database)
{
	$classes = children_names($file);
	$i = 0;

	while (count($classes) > $i) 
	{
		$table = new table();
		$table->set_name($classes[$i]);

		if (!(array_key_exists($classes[$i], $database->arrayoftables)))
		{
			$database->addtable($table);
		}

		$i+=1;
	}

	$database = uelements ($database, $file);

	foreach ($file->children() as $child) 
	{
		$database = recursivegold($child, $database);
	}

	return $database;
}

####################################################################################
########################Funkce pro opraveni chyb v databazi#########################
####################################################################################

/**
 * [funkce kontroluje zdali nekoliduje jmeno atributu s klicem]
 * @param  [database] $database [databaze]
 * @return [void]
 */
function attcheck($database)
{
	$countd = count($database->arrayoftables);

	for ($e=0; $e < $countd; $e++)
	{
		$array = array();
		$array = $database->arrayoftables;
		$val = array_values($array)[$e];

		$allKeys1 = array_keys($val->attributes);
		$allKeys2 = array_keys($val->primarykeys);

		for ($o=0; $o < count($allKeys1); $o++) 
		{ 
			for ($y=0; $y < count($allKeys2); $y++) 
			{ 
				if (strcasecmp($allKeys1[$o], $allKeys2[$y]) == 0) 
				{
					if ($allKeys1[$o] === "value") 
					{
						$better = typeenum($val->attributes[$allKeys1[$o]], $val->primarykeys[$allKeys2[$y]]);
						unset($database->arrayoftables[$val->name]->attributes[$allKeys1[$o]]);
						$database = $database->arrayoftables[$val->name]->primarykeys[$allKeys2[$y]] = $better;
					}
					else
					{
						exit(90);
					}
				}
			}
		}
	}
}

/**
 * [funkce opravuje databazi podle argumentu --etc a -b, pri argumentu -b bude kazdy nazev v tabulce mit pouze jedno zastoupeni, pri --etc se redukuji sloupce, pokud pocet sloupcu se stejnym nazvem presahne --etc (zaroven se vytvori odkaz na aktualni tabulku v tabulce s nazvem mazanych sloupcu)]
 * @param  [database] $database [databaze]
 * @return [database]           [aktualizovana databaze]
 */
function etc_b_correction($database)
{
	if($database->arguments['7'] === 1)
	{
		$countd = count($database->arrayoftables);

		for ($e=0; $e < $countd; $e++)
		{
			$array = array();
			$array = $database->arrayoftables;
			$val = array_values($array)[$e];

			$allKeys = array_keys($val->primarykeys);

			for ($i=0; $i < count($allKeys); $i++) 
			{ 
				$end = substr($allKeys[$i], -4);

				if ($end === "1_id") 
				{
					$len = strlen($allKeys[$i]) - 4;
					$name = substr($allKeys[$i], 0, $len);
					$num = 0;
					$type = 0;

					while (1) 
					{
						$num+=1;
						$name = substr($allKeys[$i], 0, $len);
						$name = $name . $num . "_id";

						if (array_key_exists($name, $val->primarykeys))
						{
							$typetemp = $val->primarykeys[$name];
							$type = typeenum($type, $typetemp);
							unset($val->primarykeys[$name]);
						}
						else
						{
							$name = substr($allKeys[$i], 0, $len);
							$name = $name . "_id";
							$val->primarykeys[$name] = $type;
							break;
						}
					}
					$database->arrayoftables[$val->name] = $val;
				}
			}
		}
	}

	if ($database->arguments['5'] !== '-1') 
	{
		if ($database->arguments['5'] === '0') 
		{
			$countd = count($database->arrayoftables);

			for ($e=0; $e < $countd; $e++)
			{
				$array = array();
				$array = $database->arrayoftables;
				$val = array_values($array)[$e];

				$nameoftable = $val->name;

				for ($y=0; $y < $countd; $y++) 
				{
					$array = array();
					$array = $database->arrayoftables;
					$cmp = array_values($array)[$y];
					
					$allKeys = array_keys($cmp->primarykeys);

					for ($i=0; $i < count($allKeys); $i++) 
					{ 
						$len = strlen($allKeys[$i]) - 4;
						$name = substr($allKeys[$i], 0, $len);
						$end1 = substr($allKeys[$i], 0, $len);
						$len = strlen($allKeys[$i]) - 3;
						$end2 = substr($allKeys[$i], 0, $len);

						if ($end1 === $nameoftable || $end2 === $nameoftable) 
						{
							$name = $cmp->name . "_id"; 
							$database->arrayoftables[$val->name]->primarykeys[$name] = "INT";

							if ($end2 === $nameoftable) 
							{
								$value = typeenum($val->primarykeys["value"], $cmp->primarykeys[$allKeys[$i]]);

								if (array_key_exists('value', $database->arrayoftables[$val->name]->primarykeys))
								{
									$database->arrayoftables[$val->name]->primarykeys['value'] = $value;
								}
								
								unset($database->arrayoftables[$cmp->name]->primarykeys[$allKeys[$i]]);
							}
							else
							{				
								$len = strlen($allKeys[$i]) - 4;
								$num = 0;

								while (1) 
								{
									$num+=1;
									$name = substr($allKeys[$i], 0, $len);
									$name = $name . $num . "_id";

									if (array_key_exists($name, $cmp->primarykeys))
									{
										$value = typeenum($database->arrayoftables[$val->name]->primarykeys['value'], $cmp->primarykeys[$allKeys[$i]]);

										if (array_key_exists('value', $database->arrayoftables[$val->name]->primarykeys))
										{
											$database->arrayoftables[$val->name]->primarykeys['value'] = $value;
										}				

										unset($database->arrayoftables[$cmp->name]->primarykeys[$name]);
									}
									else
									{
										break;
									}
								}
							}
						}
					}
				}
			}
		}
		else
		{
			$countd = count($database->arrayoftables);

			for ($e=0; $e < $countd; $e++)
			{
				$array = array();
				$array = $database->arrayoftables;
				$val = array_values($array)[$e];

				$allKeys = array_keys($val->primarykeys);

				for ($i=0; $i < count($allKeys); $i++) 
				{
					$end = substr($allKeys[$i], -4);

					if ($end === "1_id") 
					{
						$len = strlen($allKeys[$i]) - 4;
						$num = 0;
						$counter = 0;

						while (1) 
						{
							$num+=1;
							$name = substr($allKeys[$i], 0, $len);
							$name = $name . $num . "_id";

							if (array_key_exists($name, $val->primarykeys))
							{
								$counter+=1; 
							}
							else
							{
								break;
							}
						}

						if ($database->arguments['5'] < $counter) 
						{
							$len = strlen($allKeys[$i]) - 4;
							$num = 0;

							while (1) 
							{
								$num+=1;
								$name = substr($allKeys[$i], 0, $len);
								$name = $name . $num . "_id";

								if (array_key_exists($name, $val->primarykeys))
								{
									$nametemp = substr($allKeys[$i], 0, $len);
									$putter = $val->name . "_id";
									$database->arrayoftables[$nametemp]->primarykeys[$putter] = "INT";
									unset($database->arrayoftables[$val->name]->primarykeys[$name]);
								}
								else
								{
									break;
								}
							}
						}
					}
				}
			}
		}
	}
	return $database;
}

/**
 * [funkce kontroluje zdali se nachazi v databazi nejake tabulky, ktere jsou stejne pokud jejich nazvy uvazujeme case insensitive]
 * @param  [database] $database [databaze]
 * @return [database]           [databaze]
 */
function case_insensitive_cmp($database)
{
	$array = array();

	for ($e=0; $e < count($database->arrayoftables); $e++)
	{
		$array = $database->arrayoftables;
		$val = array_values($array)[$e];

		for ($i=($e + 1); $i < count($database->arrayoftables); $i++) 
		{ 
			$array = $database->arrayoftables;
			$cmp = array_values($array)[$i];

			if (!strcasecmp($cmp->name, $val->name)) 
			{
				$database = case_insensitive_merge($cmp->name, $val->name, $database);
				$e = -1;
			}
		}
	}

	return $database;
}

/**
 * [funkce spoji dve case insensiteve tabulky do jedne a tu druhou odstrani]
 * @param  [string] $first    [jmeno prvi tabulky]
 * @param  [string] $second   [jmeno druhe tabulky]
 * @param  [database] $database [databaze]
 * @return [database]           [aktualizovana databaze]
 */
function case_insensitive_merge($first, $second, $database)
{
	$table1 = $database->arrayoftables[$first];
	$table2 = $database->arrayoftables[$second];

	$allKeys = array_keys($table2->primarykeys);

	for ($i=0; $i < count($table2->primarykeys); $i++) 
	{ 
		$type = typeenum($database->arrayoftables[$first]->primarykeys[$allKeys[$i]], $database->arrayoftables[$second]->primarykeys[$allKeys[$i]]);
		$database->arrayoftables[$first]->primarykeys[$allKeys[$i]] = $type;
	}

	$allKeys = array_keys($table2->attributes);

	for ($i=0; $i < count($table2->primarykeys); $i++) 
	{ 
		$type = typeenum($database->arrayoftables[$first]->attributes[$allKeys[$i]], $database->arrayoftables[$second]->attributes[$allKeys[$i]]);
		$database->arrayoftables[$first]->attributes[$allKeys[$i]] = $type;
	}

	unset($database->arrayoftables[$second]);
	return $database;
}

function case_insentive_db_edit ($database)
{
	$array = array();
	$temp = new database();
	$temp->init($database->arguments);

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{
		$array = $database->arrayoftables;
		$val = array_values($array)[$i];

		$lower_name = mb_strtolower ($database->arrayoftables[$val->name]->name, 'UTF-8');
		$temp->arrayoftables[$lower_name] = $database->arrayoftables[$val->name];
		$temp->arrayoftables[$lower_name]->name = $lower_name;
	}

	return $temp;
}

####################################################################################
###############Funkce pro tisknuti SQL prikazu na tvorbu tabulek####################
####################################################################################

/**
 * [funkce tiskne prikazy pro vznik tabulek volanim funkci add1, add2, add3 a output]
 * @param  [database] $database [databaze]
 * @return [void]
 */
function print_database($database)
{
	$final = "";
	
	if ($database->arguments['4'] !== '-1') 
	{
		$final = $final . "--" . $database->arguments['4'] . "\n\n";
	}

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{
		$array = $database->arrayoftables;
		$val = array_values($array)[$i];

		$final = add1($final, $val->name);
		$final = add2($final, $val->primarykeys);

		if ($database->arguments['6'] === '-1')
		{
			if (count($val->attributes) === 0) 
			{
				$final = substr($final, 0, -2);
				$final = $final . "\n);\n\n";
			}
			else
				$final = add3($final, $val->attributes);
		}
		else
		{
			$final = substr($final, 0, -2);
			$final = $final . "\n);\n\n";
		}
	}

	output ($final, $database->arguments['3']);
}

/**
 * [tiskne zacatek tabulky]
 * @param  [string] $final  [string, ktery se bude tisknout]
 * @param  [string] $string [jmeno tabulky]
 * @return [string]         [obohaceny string, ktery se bude tisknout]
 */
function add1($final, $string)
{
	return $string = $final . "CREATE TABLE " . $string . "(\n   prk_" . $string . "_id" .  " INT PRIMARY KEY,\n";
}

/**
 * [tiskne podelementy tabulky]
 * @param  [string] $string [string, ktery se bude tisknout]
 * @param  [array] $arr    [pole obsahujici podelementy dane tabulky a jejich datove typy]
 * @return [string]         [obohaceny string, ktery se bude tisknout]
 */
function add2($string, $arr)
{
	$allKeys = array_keys($arr);

	for ($i=0; $i < count($arr); $i++) 
	{
		if ($allKeys[$i] === "value") 
		{
			if ($arr[$allKeys[$i]] != "0") 
			{
				$string = $string . "   " . $allKeys[$i] . " " . $arr[$allKeys[$i]] . ",\n";
			}
		}
		else
		{
			$string = $string . "   " . $allKeys[$i] . " INT,\n";
		}
	}

	return $string;
}

/**
 * [tiskne atributy tabulky]
 * @param  [string] $string [string, ktery se bude tisknout]
 * @param  [array] $arr    [pole obsahujici atributy dane tabulky a jejich datove typy]
 * @return [string]         [obohaceny string, ktery se bude tisknout]
 */
function add3($string, $arr)
{
	$allKeys = array_keys($arr);

	for ($i=0; $i < count($arr); $i++) 
	{ 
		if (($i+1) === count($allKeys)) 
			$string = $string . "   " . $allKeys[$i] . " " . $arr[$allKeys[$i]] . "\n";
		else
			$string = $string . "   " . $allKeys[$i] . " " . $arr[$allKeys[$i]] . ",\n";
	}

	return $string . ");\n\n";
}

####################################################################################
##############################Funkce pro tisknuti XML###############################
####################################################################################

/**
 * [funkce tiskne XML soubor vztahu tabulek volanim funkci xml_relationsN1, xml_relations1N a output]
 * @param  [database] $database [databaze]
 * @return [void]
 */
function xml_print ($database)
{
	$final = "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<tables>\n";

	$array = array();
	$array = $database->arrayoftables;

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{
		$val = array_values($array)[$i];

		unset($GLOBALS['array']);
		$GLOBALS['array'] = array();
		array_push($GLOBALS['array'], $val->name);

		$final = $final . "	<table name=\"" . $database->arrayoftables[$val->name]->name . "\">\n";
		$final = $final . "		<relation to=\"" . $val->name . "\" relation_type=\"1:1\" />\n";
		$final = xml_relations1N($final, $val->name, $database);
		$final = xml_relationsN1($final, $val->name, $database);
		$final = xml_relationsNM($final, $database);
		$final = $final . "	</table>\n";
	}
	$final = $final . "</tables>\n";

	output($final, $database->arguments['3']);
}

/**
 * [funkce hleda v databazi rekurzivne vztahy 1:N]
 * @param  [string] $final    [string, ktery se bude tisknout]
 * @param  [string] $name     [jmeno tabulky, pro kterou se zkouma shoda]
 * @param  [database] $database [databaze]
 * @return [string]           [obohaceny string, ktery se bude tisknout]
 */
function xml_relations1N($final, $name, $database)
{
	for ($e=0; $e < count($database->arrayoftables); $e++)
	{
		$array = array();
		$array = $database->arrayoftables;
		$val = array_values($array)[$e];

		$allKeys = array_keys($val->primarykeys);

		for ($u=0; $u < count($allKeys); $u++) 
		{
			$len = $allKeys[$u];
			$end1 = substr($allKeys[$u], 0, ($len - 4));
			$end2 = substr($allKeys[$u], 0, ($len - 3));

			if ($name === $end1 || $name === $end2) 
			{
				array_push($GLOBALS['array'], $val->name);
				$final = $final . "		<relation to=\"" . $val->name . "\" relation_type=\"1:N\" />\n";
				$final = xml_relations1N($final, $val->name, $database);
				break;
			}
		}	
	}

	return $final;
}

/**
 * [funkce hleda v databazi rekurzivne vztahy N:1]
 * @param  [string] $final    [string, ktery se bude tisknout]
 * @param  [string] $name     [jmeno tabulky, jejiz primary keys se budou tisknout]
 * @param  [database] $database [databaze]
 * @return [string]           [obohaceny string, ktery se bude tisknout]
 */
function xml_relationsN1 ($final, $name, $database)
{
	$val = $database->arrayoftables[$name];
	$allKeys = array_keys($val->primarykeys);
	$temp = array();

	for ($i=0; $i < count($allKeys); $i++) 
	{ 
		$len = $allKeys[$i];
		$end1 = substr($allKeys[$i], 0, ($len - 4));
		$end2 = substr($allKeys[$i], 0, ($len - 3));

		if (array_key_exists($end1, $database->arrayoftables)) 
		{
			array_push($temp, $end1);
		}

		if (array_key_exists($end2, $database->arrayoftables)) 
		{
			array_push($temp, $end2);
		}
	}

	$temp = array_unique($temp);

	for ($i=0; $i < count($temp); $i++) 
	{
		$val = array_values($temp)[$i];

		if (array_key_exists($val, $database->arrayoftables)) 
		{
			array_push($GLOBALS['array'], $val);
			$final = $final . "		<relation to=\"" . $val . "\" relation_type=\"N:1\" />\n";
			$final = xml_relationsN1($final, $val, $database);
		}
	}

	return $final;
}

/**
 * [jelikoz predchozi fce prohledali kardinality 1:N a N:1, vsechny ostatni jsou N:M, a ty uklada do stringu tahle fce]
 * @param  [string] $final    [string, ktery se bude tisknout]
 * @param  [database] $database [databaze]
 * @return [string]           [obohaceny string, ktery se bude tisknout]
 */
function xml_relationsNM ($final, $database)
{
	$allKeys = array();

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{ 
		$array = array();
		$array = $database->arrayoftables;
		$val = array_values($array)[$i];
		array_push($allKeys, $val->name);
	}

	$allKeys = array_keys($database->arrayoftables);

	$result = array_diff($allKeys, $GLOBALS['array']);
	for ($i=0; $i < count($result); $i++) 
	{ 
		$name = array_values($result)[$i];
		$final = $final . "		<relation to=\"" . $name . "\" relation_type=\"N:M\" />\n";
	}
	
	return $final;
}

####################################################################################
##################################Ostatni funkce####################################
####################################################################################

/**
 * [kontrola cisla, zda-li je float]
 * @param  [float] $value [kontrolovana hodnota]
 * @return [bool]        [pokud je cislo float, vraci true, jinak false]
 */
function isfloat($value)
{
	return is_float($value + 0);
}

/**
 * [funkce porovna dva datove typy a vrati ten vetsi]
 * @param  [string] $type     [prvni porovnavany datovy typ]
 * @param  [string] $typetemp [druhy porovnavany datovy typ]
 * @return [string]           [vetsi porovnavany datovy typ]
 */
function typeenum ($type, $typetemp)
{
	$typec = $type;
	$typetempc = $typetemp;

	if ($type === "BIT") 
		$type = 1;
	if ($type === "INT") 
		$type = 2;
	if ($type === "FLOAT") 
		$type = 3;
	if ($type === "NVARCHAR") 
		$type = 4;
	if ($type === "NTEXT") 
		$type = 5;

	if ($typetemp === "BIT") 
		$typetemp = 1;
	if ($typetemp === "INT") 
		$typetemp = 2;
	if ($typetemp === "FLOAT") 
		$typetemp = 3;
	if ($typetemp === "NVARCHAR") 
		$typetemp = 4;
	if ($typetemp === "NTEXT") 
		$typetemp = 5;

	if ($typetemp < $type) 
		return $typec;
	else
		return $typetempc;
}

/**
 * [finalni funkce pro tisk jak SQL prikazu, tak XML souboru, je to jedno jelikoz funkce dostava na vstup pouze naformatovany string]
 * @param  [string] $final     [string pro tisk]
 * @param  [string] $parameter [parametr --ouput]
 * @return [void]
 */
function output ($final, $parameter)
{
	if ($parameter === '-1') 
	{
		echo $final;
		exit(0);
	}
	else
	{
		if (file_exists($parameter))
			exit(3);

		$path_parts = pathinfo($parameter);
		
		if (!file_exists($path_parts['dirname'])) 
		{
    		mkdir($path_parts['dirname'], 0777, true);
		}

		realpath($parameter);

		$output = fopen($parameter,'w');
		fwrite($output, $final);
		fclose($output);
		exit(0);
	}
}

?>
