<?php
/**
 * @author Petr Buchal(xbucha02)
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

function help_test($argv, $argc)
{
	$index = 1;

	while ($index < $argc) 
	{
		if (!(strcmp ($argv[$index], "--help")))
		{
			return 1;
		}
		$index++;
	}

	return 0;
}

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

function isfloat($value)
{
	return is_float($value + 0);
}

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

function add1($final, $string)
{
	return $string = $final . "CREATE TABLE " . $string . "(\n	prk_" . $string . "_id" .  " INT PRIMARY KEY,\n";
}

function add2($string, $arr)
{
	for ($i=0, $e=0; $e < count($arr); $i++) 
	{
		if (!empty($arr[$i])) 
		{
			$string = $string . "	" . $arr[$i] . "_id INT,\n";
			$e++;
		}
	}

	return $string;
}

function add3($string, $arr, $arr2)
{
	for ($i=0, $e=0; $e < count($arr); $i++) 
	{
		if (!empty($arr[$i])) 
		{
			$e++;
			if ($e === count($arr)) 
				$string = $string . "	" . $arr[$i] . " " . $arr2[$i] . "\n";
			else
				$string = $string . "	" . $arr[$i] . " " . $arr2[$i] . ",\n";
		}
	}

	return $string . ");\n";
}

function emptyfile($name)
{
	if (file_exists($name)) 
	{
		if (filesize($name) === 0)
	    	exit (1);

	    $xml = simplexml_load_file($name);

	 	if ($xml->count() === 0) 
	 	{
	 		exit(0);
	 	}
	} 
}

function childrennames ($file)
{
	$namescount = $file->count();
	$namesarr = array_fill(0, $namescount, 0);
	$i = 0;

	foreach ($file->children() as $child)
	{
	    $namesarr[$i] = $child->getName();
	    $i+=1;
	}

	$namesarr = array_unique($namesarr);
	return $namesarr;
}

function childrennames2 ($arr,$file)
{
	$count = $file->count();
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{
		$ncount+= $file->{$arr[0]}[$i]->count() . "\n";
	}

	$namesarr = array_fill(0, $ncount, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++)
	{
		foreach ($file->{$arr[0]}[$i]->children() as $childreen)
		{
			 $namesarr[$ncount] = $childreen->getName();
			 $ncount+=1;
		}
		$namesarr = array_unique($namesarr);
	}

	return $namesarr;
}

function getattributes($arr, $file)
{
	$count = $file->count();
	$namesarr = array_fill(0, $count, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{ 
		foreach($file->{$arr[0]}[$i]->attributes() as $a => $b)
		{
			$ncount+= 1;
		}
	}

	$namesarr = array_fill(0, $ncount, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{ 
		foreach($file->{$arr[0]}[$i]->attributes() as $a => $b)
		{
			$namesarr[$ncount] = $a;
			$ncount+=1;

		}
		$namesarr = array_unique($namesarr);
	}

	return $namesarr;
}

function getattributesval($arr, $file)
{
	$count = $file->count();
	$namesarr = array_fill(0, $count, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{ 
		foreach($file->{$arr[0]}[$i]->attributes() as $a => $b)
		{
			$ncount+= 1;
		}
	}

	$namesarr = array_fill(0, $ncount, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{ 
		foreach($file->{$arr[0]}[$i]->attributes() as $a => $b)
		{
			$namesarr[$ncount] = $b;
			$ncount+=1;

		}
	}

	return $namesarr;
}

function childrenvalues ($arr, $file)
{
	$count = $file->count();
	$ncount = 0;

	for ($i=0; $i < $count; $i++) 
	{
		$ncount+= $file->{$arr[0]}[$i]->count();
	}

	$namesarr = array_fill(0, $ncount, 0);
	$ncount = 0;

	for ($i=0; $i < $count; $i++)
	{
		foreach ($file->{$arr[0]}[$i]->children() as $childreen)
		{
			 $namesarr[$ncount] = $childreen;
			 $ncount+=1;
		}
	}

	return $namesarr;
}

function control($name, $val, $type)
{
	for ($i=0, $e=0; $e < count($name); $i++) 
	{
		if (!empty($name[$i]))
		{
			if (empty($val[$i]) || $val[$i] === '0' || $val[$i] === '1' || !strcasecmp($val[$i], "true")|| !strcasecmp($val[$i], "false")) 
			{
				$val[$i] = "BIT";
			}
			else if (isfloat("$val[$i]")) 
			{
				$val[$i] = "FLOAT";
			}
			else if (is_numeric("$val[$i]")) 
			{
				$val[$i] = "INT";
			}
			else if ($type === 1) 
			{
				$val[$i] = "NVARCHAR";
			}
			else
			{
				$val[$i] = "NTEXT";
			}
			$e++;
		}
		else
		{
			$val[$i] = "";
		}
	}
	return $val;
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

$types = array("2"=>"-1", "3"=>"-1", "4"=>"-1", "5"=>"-1", "6"=>"-1", "7"=>"-1", "8"=>"-1", );

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
		if(help_test($argv, $argc))
			exit(1);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);
	    break;

	case ($argc === 4):
		if(help_test($argv, $argc))
			exit(1);

		$type1 = parameter_test($argv[1]);
		$types[$type1] = parameter_value($type1, $argv[1]);

		$type2 = parameter_test($argv[2]);
		$types[$type2] = parameter_value($type2, $argv[2]);

		$type3 = parameter_test($argv[3]);
		$types[$type3] = parameter_value($type3, $argv[3]);
	    break;

	case ($argc === 5):
		if(help_test($argv, $argc))
			exit(1);

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
		if(help_test($argv, $argc))
			exit(1);

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
		if(help_test($argv, $argc))
			exit(1);

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
		if(help_test($argv, $argc))
			exit(1);

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


emptyfile($types['2']);  //kontrola prazdnosti souboru


$file = simplexml_load_file($types['2']);  //nacteni souboru
$names = childrennames($file);  //jmeno hlavni tabulky
$final = "";
$final = add1($final, $names[0]);  //CREATE_TABLE


$children = childrennames2($names, $file);
$childrenval = childrenvalues($names, $file);
$childrenval = control($children, $childrenval, 0);
$final = add2($final, $children);  //tisk potomku


$attributes = getattributes($names, $file);
$attributesval = getattributesval($names, $file);
$attributesval = control($attributes, $attributesval, 1);


if ($types['6'] === '-1')  //osetreni argumentu -a
{
	$final = add3($final, $attributes, $attributesval);  //tisk atributu
}
else
{
	$final = substr($final, 0, -2);
	$final = $final . "\n);\n";
}


for ($i=0, $e=0; $e < count($children); $i++) 
{ 
	if (!empty($children[$i]))
	{
		$array = array("value");
		$final = add1($final, $children[$i]);
		$final = add3($final, $array, $childrenval);
		$e++;
	}
}

output ($final, $types['3']);

?>
