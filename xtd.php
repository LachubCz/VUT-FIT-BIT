<?php
/**
 * @author Petr Buchal(xbucha02)
 */

//####################################################################################
//############################Zpracovani argumentu skriptu############################
//####################################################################################

//tisk cele napovedy
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

//pokud je nektery z argumentu "--help" a argumentu je vic nez dva, ukoncuje program "exit(1)"
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

//vraci hodnotu parametru, pokud je parametr jiny nez v zadani ukoncuje program "exit(1)"
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

//kontrola cisla, zda-li je float
function isfloat($value)
{
	return is_float($value + 0);
}

//vraci hodnoty parametru skriptu, pokud je hodnota nespravna program konci "exit(1)"
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

//vytvoreni pole do ktereho se ukladaji hodnoty argumentu
$types = array("2"=>"-1", "3"=>"-1", "4"=>"-1", "5"=>"-1", "6"=>"-1", "7"=>"-1", "8"=>"-1", );

//switch, ktery zpracovava argumenty pomoci predchozich funkci
switch(true)
{
	case ($argc === 1):
		break;

	case ($argc === 2):
	  	if (!(strcmp ($argv[1], "--help")))
	  	{
	  	  	help_print();
	  		exit(0);
	  	}

	  	$type1 = parameter_test($argv[1]);
	  	$types[$type1] = parameter_value($type1, $argv[1]);
	  	break;

	case ($argc === 3):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);
	    break;

	case ($argc === 4):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);
	    break;

	case ($argc === 5):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);

		$type4 = parameter_test($argv[4]);
		$types[$type4] = parameter_value($type4, $argv[4]);
	    break;

	case ($argc === 6):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);

		$type4 = parameter_test($argv[4]);
		$types[$type4] = parameter_value($type4, $argv[4]);

		$type5 = parameter_test($argv[5]);
		$types[$type5] = parameter_value($type5, $argv[5]);
	    break;

	case ($argc === 7):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);

		$type4 = parameter_test($argv[4]);
		$types[$type4] = parameter_value($type4, $argv[4]);

		$type5 = parameter_test($argv[5]);
		$types[$type5] = parameter_value($type5, $argv[5]);

		$type6 = parameter_test($argv[6]);
		$types[$type6] = parameter_value($type6, $argv[6]);
	    break;

	case ($argc === 8):
		help_test($argv, $argc);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);

		$type4 = parameter_test($argv[4]);
		$types[$type4] = parameter_value($type4, $argv[4]);

		$type5 = parameter_test($argv[5]);
		$types[$type5] = parameter_value($type5, $argv[5]);

		$type6 = parameter_test($argv[6]);
		$types[$type6] = parameter_value($type6, $argv[6]);

		$type7 = parameter_test($argv[7]);
		$types[$type7] = parameter_value($type7, $argv[7]);
	    break;

	default:
		exit(1);
}
//####################################################################################
//############################Zpracovani vstupniho souboru############################
//####################################################################################

//kontrola prazdnosti souboru a vstupu STDIN + kontrola existence souboru
function emptyfile($name, $type)
{
	if ($type === '1') 
	{
		$xml = simplexml_load_string($name);

	 	if ($xml->count() === 0) 
	 	{
	 		exit(0);
	 	}
	}
	else
	{
		if (file_exists($name)) 
		{
			if (filesize($name) === 0)
		    	exit (0);

		    $xml = simplexml_load_file($name);

		 	if ($xml->count() === 0) 
		 	{
		 		exit(0);
		 	}
		}
		else
		{
			exit(1);
		}
	}
}

//nacteni a rozparserovani souboru, pokud neni soubor zadan nacteni dat ze STDIN a jejich rozparserovani
function fileload($file)
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
		emptyfile($file, '1');  //kontrola prazdnosti souboru
		$file = simplexml_load_string($file);  //nacteni souboru
	}
	else
	{
		emptyfile($file, '0');  //kontrola prazdnosti souboru
		$file = simplexml_load_file($file);  //nacteni souboru
	}

	return $file;
}

//####################################################################################
//##################################Funkce pro tisk###################################
//####################################################################################

//tiskne tabulku volanim funkci add1, add2, add3
function print_database($database)
{
	$final = "";
	
	if ($database->arguments['4'] !== '-1') 
	{
		$final = $final . "--" . $header . "\n\n";
	}
	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{ 
		$array = $database->arrayoftables;
		$val = array_values($array)[$i];
		$final = add1($final, $val->name);
		$final = add2($final, $val->primarykeys);

		if ($database->arguments['6'] === '-1')  //osetreni argumentu -a
		{
			$final = add3($final, $val->attributes);
		}
		else
		{
			$final = substr($final, 0, -2);
			$final = $final . "\n);\n";
		}
	}
	output ($final, $database->arguments['3']);
}

//tiskne zacatek tabulky
function add1($final, $string)
{
	return $string = $final . "CREATE TABLE " . $string . "(\n	prk_" . $string . "_id" .  " INT PRIMARY KEY,\n";
}

//tiskne podelementy tabulky
function add2($string, $arr)
{
	$allKeys = array_keys($arr);
	/*
	if (count($arr) === 0) 
	{
		$string = $string . "	" . "value " . "NTEXT\n";
	}
	*/
	for ($i=0; $i < count($arr); $i++) 
	{
		$string = $string . "	" . $allKeys[$i] . " INT,\n";
	}

	return $string;
}

//tiskne atributy tabulky
function add3($string, $arr)
{
	$allKeys = array_keys($arr);
	for ($i=0; $i < count($arr); $i++) 
	{ 
		if (($i+1) === count($allKeys)) 
			$string = $string . "	" . $allKeys[$i] . " " . $arr[$allKeys[$i]] . "\n";
		else
			$string = $string . "	" . $allKeys[$i] . " " . $arr[$allKeys[$i]] . ",\n";
	}

	return $string . ");\n";
}

function output ($final, $parameter)
{
	if ($parameter === '-1') 
	{
		echo $final;
	}
	else
	{
		$output = fopen($parameter,"w");
		fwrite($output, $final);
		fclose($output);
	}
}

//####################################################################################
//#############################Tridy pro ukladani hodnot##############################
//####################################################################################

//trida na ulozeni pole tabulek
class database
{
	var $arrayoftables;
	var $arguments;

	function init($types)
	{
		$this->arrayoftables = array();
		$this->arguments = $types;
	}

	function addtable ($table)
	{
		$this->arrayoftables[$table->name] = $table;
	}
}

//trida na ulozeni jednotlivych tabulek 
class table
{
	var $name;
	var $primarykeys;
	var $attributes;

	function set_name($name)
	{
		$this->name = $name;
		$this->primarykeys = array();
		$this->attributes = array();
	}
}

//####################################################################################
//####################################Parsing XML#####################################
//####################################################################################

function childrennames ($file)
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

function control($val, $type)
{
	if (empty($val) || $val === '0' || $val === '1' || !strcasecmp($val, "true")|| !strcasecmp($val, "false")) 
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

function attredit($arrdata, $arrnew)
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

function attuniq($array, $name, $type)
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

//funkce zatim neumi rearangovat atributy prekracujici etc do svych tabulek
function etcdesider($name, $arrayprk, $database)
{
	$etc = $database->arguments['5'];
	$array = $database->arrayoftables;

	for ($i=0; $i < count($database->arrayoftables); $i++) 
	{ 
		$val = array_values($array)[$i];

		if ($val->name === $name) 
		{
			$primarykeys = attredit($val->primarykeys, $arrayprk);
			$database->arrayoftables[$name]->primarykeys = $primarykeys;
		}	
	}
	//rozhodne bud o ponechani atributu (pouziti fce etctbmaker) nebo o vytvoreni prislusne nove tabulky
}

function uelements ($database, $file)
{
	$countf = count($file);
	$countd = count($database->arrayoftables);
	for ($e=0; $e < $countd; $e++) //prochazi tabulky v arrayoftables dokud tam nejake jsou
	{
		$array = array();
		$array = $database->arrayoftables;  //priradi do array arrayoftebles
		$val = array_values($array)[$e];  //vybere e-tou hodnotu
		for ($i=0; $i < count($file->{$val->name}); $i++)
		{
			$array = array();
			if ($database->arguments['6'] === '-1') 
			{
				foreach($file->{$val->name}[$i]->attributes() as $a => $b)
				{
					//echo $a ." --- " . $b . "\n";
					$type = control($b, '1');
					$database->arrayoftables[$val->name]->attributes[$a] = $type;
					//argput(, $a, $type, $database);
				}
			}

			foreach ($file->{$val->name}[$i]->children() as $childreen)  //prochazeni jmen a hodnot country a jinych struktur ktere jsou prvni na rade
			{
				$name = $childreen->getName();
				//echo $childreen . "\n";
				$type = control($childreen, '0');
				$array = attuniq($array, $name, $type);  //musime se podivat zdali pole uz hodnotu obsahuje pokud ano, preidexuje se pole
			}
			etcdesider($val->name, $array, $database);
			//nyni by se mela zavolat funkce na zpracovani pole do tvaru num_ID podle etc
			//a mely by se aktualizovat hodnoty ve strukture tabulek
		}
	}
	return $database;
}

function recursivegold ($file, $database)
{
	$classes = childrennames($file);  //prohledani prvni vrstvy
	$i = 0;

	while (count($classes) > $i) 
	{
		$table = new table();
		$table->set_name($classes[$i]);
		$database->addtable($table);
		$i+=1;
	}

	$database = uelements ($database, $file);
	foreach ($file->children() as $child) 
	{
		$database = recursivegold($child, $database);
	}
	return $database;
}

//####################################################################################
//###########################Samotne telo mocneho programu############################
//####################################################################################

//osetreni vzajemneho zadani argumentu --etc a -a
if (($types['5'] !== '-1') && ($types['7']=== '1')) 
{
	exit(1);
}

$file = fileload($types['2']);  //pokud soubor neni zadan, je prijman standartni vstup + kontrola zda-li neni soubor prazdny a existuje

//vytvoreni hlavni struktury databaze, 
$database = new database();
$database->init($types);

$database = recursivegold ($file, $database);

print_database($database);

?>
