<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/*
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2011, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */

	if(!function_exists('ago'))
	{
		/**
		 * Time ago
		 * 
		 * @example 
		 * // Show how much time has passed, with the highest u.m.
		 * ago(1333361278);
		 * // Show how much time has passed, with all u.m.
		 * ago(1333361278,true);
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/ago
		 * 
		 * @param string $time - Unix Timestamp
		 * @param bool $rcs - Wether to show recursive time, up to the second
		 * @return string - How much time has passed
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function ago($time,$rcs = false)
		{
			// Store the current time
			$cur_tm = time(); 
			
			// Get the time difference
			$dif = $cur_tm-$time;
			
			// Get the units of measuerement
			$pds = array('second','minute','hour','day','week','month','year','decade');
			$lngh = array(1,60,3600,86400,604800,2630880,31570560,315705600);
			
			// Calculate the highest u.m.
			for($v = sizeof($lngh)-1; ($v>=0)&&(($no = $dif/$lngh[$v])<=1); $v--); 
			if($v < 0) $v = 0; 
			
			// Get the time left
			$_tm = $cur_tm-($dif%$lngh[$v]);
			
			// Format the number
			$no = floor($no); 
			if($no <> 1) $pds[$v] .='s'; 
			$x=sprintf("%d %s ",$no,$pds[$v]);
			
			// Recursive call?
			if(($rcs)&&($v>=1)&&(($cur_tm-$_tm) > 0))
			{
				$x .= ago($_tm);
			}
			
			// All done
			return $x;
		}// end function ago()
	}

	if(!function_exists('array_depth'))
	{
		/**
		 * Calculate the depth of an array
		 * 
		 * @example 
		 * // Returns 1
		 * array_depth(array('foo','bar'));
		 * // Returns 2
		 * array_depth(array('foo',array('bar','baz')));
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/array_depth
		 * @param array $array
		 * @return int - array depth
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function array_depth($array)
		{
			// Not an array? 0 depth
			if (!is_array($array))
			{
				return 0;
			}
			
			// Start with 1
		    $max = 1;
		    
		    // Use the print_r function for this
		    foreach (explode("\n", print_r($array, true)) as $line) 
		    {
		        $i = (strlen($line)-strlen(ltrim($line)))/4;
		        $max = $i>$max?$i:$max;
		    }
		
		    // Done
		    return intval(ceil(($max-1)/2)+1);
			
		}// end function array_depth()
	}
	
	if(!function_exists('array_flat'))
	{
		/**
		 * Flatten an array to 1 level; unique values;
		 * 
		 * @example 
		 * // Return 'a','b','c'
		 * array_flat(array('a',array('b','c')));
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/array_flat
		 * @param array $array
		 * @return array - flattened array
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function array_flat($array=array())
		{
			$r = array();
			
			foreach ($array AS $a)
			{
				if (!is_array($a))
				{
					$r[] = $a;
				}
				else 
				{
					$r = array_merge($r,array_flat($a));
				}
			}
			
			return array_unique($r);
			
		}// end function array_flat()
	}
	
	if(!function_exists('array_structure'))
	{
		/**
		 * Structure an array according to the serialized rules
		 * 
		 * @example 
		 * # $array = array('a','b','c','d','e');
		 * # $rules = array('root',0,1,1,2);
		 * # Returns array('a'=>array('b'=>array(3=>'d','c'=>array(4=>'e'))));
		 * $result = array_structure($array,$rules);
		 * @link http://octoms.com/doc/helper/recursion.inc/array_structure
		 * 
		 * @param array $array - 1D List of items
		 * @param array $rules - 1D Rules of placement; must be in ASC order, 'root' being lowest;
		 * Each value in $rules shows the parent key for the corresponding item in $array;
		 * Must begin with 'root'.
		 * @return multidimensional array
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function array_structure($array,$rules)
		{			
			// Prepare the result
			$result = array();
			foreach ($rules AS $key => $value)
			{
				if ($value === 'root')
				{
					$result[$key] = $array[$key];
				}
				elseif (isset($result[$value])) 
				{
					
					// Prepare the new rules
					$nr = array();
					foreach ($rules AS $k => $v)
					{
						// Replace the current value with "root"
						if ($v === $value)
						{
							$nr[$k] = 'root';
						}
						// And remove old roots
						elseif (is_numeric($v))
						{
							$nr[$k] = $rules[$k];
						}
					}
					unset($result[$value]);
					$result[$array[$value]] = array_structure($array,$nr,true);
				}
			}
			return $result;
		}// end function array_structure()
	}
	
	if (!function_exists('array_to_object'))
	{
		/**
		 * Array to object - Transform an array into an object recursively
		 * 
		 * @example 
		 * // Transform array $array to an object
		 * $obj = array_to_object($array);
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/array_to_object
		 * @param array $data
		 * @return object/mixed - Returns the input $data unaltered if it
		 * is not an array (it could be an integer, a resource etc.)
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function array_to_object($data=NULL)
		{
			if(is_array($data) || is_object($data))
			{
				$result = new stdClass(); 
				foreach ($data as $key => $value)
				{ 
					$result->$key = array_to_object($value); 
				}
				return $result;
			}
			return $data;
			
		}// end function array_to_object()
	}
	
	if(!function_exists('array_to_xml'))
	{
		class SimpleXMLExtended extends SimpleXMLElement
		{
			public function addCData($cdata_text)
			{
				$node= dom_import_simplexml($this);
				$no = $node->ownerDocument;
				$node->appendChild(
					$no->createCDATASection(
						$cdata_text
					)
				);
			}
		}
			
		/**
		 * Turn an array to an XML string
		 * 
		 * @example 
		 * // Get a simple xml <baz><foo><![CDATA[bar]]></foo></baz>
		 * array_to_xml(array('foo'=>'bar'),'baz');
		 * // Add some attributes <baz><foo bar="baz" abc="def"><![CDATA[bar]]></foo></baz>
		 * array_to_xml(array('foo:bar=baz,abc=def'=>'bar'),'baz');
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/array_to_xml
		 * 
		 * @param array $array
		 * @param string $name
		 * @return string/boolean - XML formatted string OR False on failure
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function array_to_xml($array,$name=null,&$xml=null)
		{
			// Set the top XML
			if ($name != null)
			{
				$xml = new SimpleXMLExtended(
					'<?xml version="1.0"?><'.$name.'></'.$name.'>',
					LIBXML_NOCDATA
				);
			}
			
			// We need the XML
			if (is_null($xml))
			{
				return false;
			}

			// Parse the array
			foreach($array as $key => $value)
			{
				// Get attributes, if they were set
				if (false !== strpos($key, ':'))
				{
					// Get the attributes
					$attributes = substr($key, strpos($key, ':')+1);
					
					// Cleant the key
					$key = substr($key, 0,strpos($key, ':'));
					
					// Split the attributes
					$attributes = explode(',', $attributes);
				}
				else 
				{
					$attributes = array();
				}

				// Parse the array
				if(is_array($value))
				{
					if(!is_numeric($key))
					{
						$subnode = $xml->addChild($key);
						array_to_xml($value, null, $subnode);
					}
					else
					{
						$subnode = $xml->addChild('key_'.$key);
						array_to_xml($value, null, $subnode);
					}
				}
				else 
				{
					if(!is_numeric($key))
					{
						$subnode = $xml->addChild($key);
						$subnode->addCData($value);
					}
					else
					{
						$subnode = $xml->addChild('key_'.$key);
						$subnode->addCData($value);
					}
				}
				
				// Append the extra data
				if (count($attributes)>0)
				{
					foreach ($attributes AS $a)
					{
						$exp = explode('=', $a);
						if (!isset($a[1])) $a[1] = '';
						$subnode->addAttribute($exp[0], $exp[1]);
					}
				}
		    }
		    
		    // All done
		    if ($name != null)
		    {
		    	return $xml->asXML();
		    }
			
		}// end function array_to_xml()
	}
	
	if(!function_exists('ini'))
	{
		/**
		 * Verify if an INI value was set to TRUE
		 * 
		 * @example 
		 * // Verify "allow_url_fopen"
		 * ini("allow_url_fopen");
		 * @link http://octoms.com/doc/helper/recursion.inc/ini
		 * 
		 * @param string $setting - PHP.INI directive
		 * @return NULL if not set, TRUE if activated, FALSE if deactivated, String if other value is provided
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function ini($setting=NULL)
		{
			// We need this!
			if(is_null($setting))
			{
				return null;
			}
			else
			{
				// The user set this
				if ("" !== $v = ini_get($setting))
				{
					if (in_array(strtolower($v), array('1','on')))
					{
						return TRUE;
					}
					elseif (in_array(strtolower($v), array('0','off')))
					{
						return FALSE;
					}
					else 
					{
						return $v;
					}
				}
				// This was not set
				else 
				{
					return null;
				}
			}
		}// end function ini()
	}
	
	if (!function_exists('object_to_array'))
	{
		/**
		 * Object to array - Transform an object into an array recursively
		 * 
		 * @example 
		 * // Transform object $obj to an array
		 * $array = object_to_array($obj);
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/object_to_array
		 * 
		 * @param object $data
		 * @return array/mixed - Returns the input $data unaltered if it
		 * is not an object (it could be an integer, a resource etc.)
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function object_to_array($data=NULL)
		{
			if(is_array($data) || is_object($data))
			{
				$result = array(); 
				foreach ($data as $key => $value)
				{ 
					$result[$key] = object_to_array($value); 
				}
				return $result;
			}
			return $data;
			
		}// end function object_to_array()
	}
	
	if(!function_exists('remove_dir'))
	{
		/**
		 * Completely remove a directory and all files in it
		 * 
		 * @example 
		 * // Remove /uploads/folder
		 * remove_dir('/uploads/folder');
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/remove_dir
		 * 
		 * @param $dir
		 * @return boolean - TRUE on success, FALSE on failure
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function remove_dir($dir=null)
		{
		    if (!is_null($dir) && FALSE !== $dh = opendir($dir))
		    {
			    while(FALSE !== ($file = @readdir($dh)))
			    {
			        if(!in_array($file, array('.','..')))
			        {
			            @chmod($dir.DS.$file, 0777);
			            if(is_dir($dir.DS.$file))
			            {
			                remove_dir($dir.DS.$file);
			            }
			            else
			            {
			            	unlink($dir.DS.$file);
			            }
			        }
			    }
			    closedir($dh);
			    @rmdir($dir);
			    return TRUE;
		    }
		    else 
		    {
		    	return FALSE;
		    }
		}// end function remove_dir()
	}
	
	if(!function_exists('typeof'))
	{
		/**
		 * Get the type of a variable
		 * 
		 * @example 
		 * // Returns 'string'
		 * typeof("abc");
		 * 
		 * @link http://octoms.com/doc/helper/recursion.inc/typeof
		 * 
		 * @param mixed $var
		 * @return string - the variable type
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function typeof($var=null)
		{
			return gettype($var);
		}// end function typeof()
	}
	
/* End Of File <recursion.inc> */