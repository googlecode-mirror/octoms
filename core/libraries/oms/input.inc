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
	
	/*
	 * Input class
	 * 
	 * @link http://octoms.com/doc/input
	 * 
	 * @package OctoMS
	 * @subpackage input
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class input_cl
	{
		
		/*
		 * Set the default upload location relative to the ROOT
		 * The constructor will change this to ROOT.DS.{}
		 */
		var $uploadDir = 'upload';
		
		/*
		 * Class constructor
		 *  
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function __construct()
		{
			// Fix quotes gpc settings; one time only
			if (!isset($this->fix_gpc))
			{
				if (get_magic_quotes_gpc()) 
				{
				    $process = array(&$_GET, &$_POST, &$_COOKIE, &$_REQUEST);
				    while (FALSE !== list($key, $val) = each($process)) 
				    {
				        foreach ($val as $k => $v) 
				        {
				            unset($process[$key][$k]);
				            if (is_array($v)) 
				            {
				                $process[$key][stripslashes($k)] = $v;
				                $process[] = &$process[$key][stripslashes($k)];
				            } 
				            else 
				            {
				                $process[$key][stripslashes($k)] = stripslashes($v);
				            }
				        }
				    }
				    unset($process);
				    
				    // Performed quotes gpc fix 
				    $this->fix_gpc = true;
				}
				else 
				{
					// Quotes gpc fix was not needed
					$this->fix_gpc = false;
				}
			}

			// Set the upload directory
			$this->uploadWeb = (WEBROOT!=WS?WEBROOT.WS:WS).$this->uploadDir;
			$this->uploadDir = ROOT.DS.$this->uploadDir;
			
		}// end function __construct()()
		
		/**
		 * Get super globals
		 * 
		 * @example # Return $_POST["k"]
		 * $this->input->sg('post','k');
		 * # Return $_GET['a'] and $_GET['b']
		 * $this->input->sg('get',array('a','b'));
		 * # Return $_ENV
		 * $this->input->sg('env');
		 * @link http://octoms.com/doc/input_cl/sg
		 * 
		 * @method sg()
		 * @param string $superGlobal - (get, post, server or env)
		 * @param string/array $val
		 * @return string/array/null<br/>
		 * String - if $var is a string and {superGlobal}[$var] exists<br/>
		 * Array - if $var is an array and some of {superGlobal}[$key] exist<br/>
		 * NULL - if the key was not set
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function sg($superGlobal=NULL,$val=NULL)
		{
			// Format the SuperGlobal's name
			if (empty($superGlobal))
			{
				return NULL;
			}
			else 
			{
				$sg = trim(strtoupper($superGlobal));
				switch ($sg)
				{
					case 'GET':		$sg = $_GET; break;
					case 'POST':	$sg = $_POST; break;
					case 'SERVER':	$sg = $_SERVER; break;
					case 'ENV':		$sg = $_ENV; break;
					default: $sg = array();
				}
			}
			
			// Nothing to return
			if (count($sg) == 0)
			{
				return NULL;
			}
			
			// Get all the keys
			if (empty($val))
			{
				return $sg;
			}
			else 
			{
				if (is_array($val))
				{
					$arr = array();
					foreach ($val AS $v)
					{
						if (isset($sg[$v]))
						{
							$arr[$v] = $sg[$v];
						}
						else 
						{
							$arr[$v] = NULL;
						}
					}
					return $arr;
				}
				elseif (is_string($val) || is_numeric($val)) 
				{
					if (isset($sg[$val]))
					{
						return $sg[$val];
					}
				}
				else 
				{
					return null;
				}
			}
			
		}// end function sg()
		
		/**
		 * Return _GET variables without unsetting this superglobal
		 * 
		 * @example # Return $_GET["key"]
		 * $this->input->get("key");
		 * # Return $_GET["k1"] and $_GET["k2"]
		 * $this->input->get(array("k1","k2"));
		 * # Return true if $_GET is set, false otherwise
		 * $this->input->get();
		 * # Ruturn the keys of $_GET
		 * $this->input->get(true);
		 * @link http://octoms.com/doc/input/get
		 * 
		 * @method get()
		 * @param string/array $var - $_GET key(s) to return
		 * @param array $clean - clean filters to apply
		 * @return string/array/null<br/>
		 * String - if $var is a string and $_GET[$var] exists<br/>
		 * Array - if $var is an array and some of $_GET[$key] exist<br/>
		 * NULL - if the key was not set
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function get($var=NULL,$clean=array())
		{
			if ($var === true) return array_keys($_GET);
			if ($var === null) return (isset($_GET) && count($_GET)>0)?true:false;
			$r = func_get_args();if(!isset($r[2]))$var=$this->sg('get',$var);
			
			if (is_array($var))
			{
				$result = array();
				foreach ($var AS $k=>$v)
				{
					if (!is_null($v) && !is_null($v = $this->get($v,$clean,TRUE)))
					{
						$result[$k] = $v;
					}
				}
				return $result;
			}
			else
			{
				// Maybe just one filter?
				if (is_string($clean)) $clean = array($clean);
				
				// Apply the filters
				if (is_array($clean) && count($clean) > 0)
				{
					foreach ($clean AS $filter)
					{
						if (method_exists($this, 'clean_'.$filter))
						{
							$var = call_user_func_array(array($this, 'clean_'.$filter), array($var));
						}
					}
				}
			}
			
			return urldecode($var);
		}// end function get()
		
		/**
		 * Delete a key
		 * 
		 * @example 
		 * # Remove the $_POST['username']
		 * $this->input->del('post','username');
		 * # Remove the $_POST['username'] and $_POST['password']
		 * $this->input->del('post',array('username','password'));
		 * @link http://octoms.com/doc/input_cl/del
		 * 
		 * @param string $sg - SuperGlobal (GET, POST, SERVER or ENV)
		 * @param string $key
		 * @return $this
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function del($sg=null,$key=null)
		{
			// Must be a proper superglobal
			if (!in_array(strtoupper($sg),array('GET','POST','SERVER','ENV'))) return $this;
			
			// Unset the value
			if (is_array($key)) 
			{
				foreach ($key AS $k)
				{
					@eval('unset($_'.strtoupper($sg).'[$k]);');
				}
			}
			elseif (!is_null($key))
			{
				@eval('unset($_'.strtoupper($sg).'[$key]);');
			}
			
			// All done
			return $this;
			
		}// end function del()
		
		/**
		 * Return _POST variables without unsetting this superglobal
		 * 
		 * @example # Return $_POST["key"]
		 * $this->input->post("key");
		 * # Return $_POST["k1"] and $_POST["k2"]
		 * $this->input->post(array("k1","k2"));
		 * # Return true if $_POST is set, false otherwise
		 * $this->input->post();
		 * # Return the $_POST keys
		 * $this->input->post(true);
		 * @link http://octoms.com/doc/input_cl/post
		 * 
		 * @method post()
		 * @param string/array $var - $_POST key(s) to return
		 * @param array $clean - clean filters to apply
		 * @return string/array/null<br/>
		 * String - if $var is a string and $_POST[$var] exists<br/>
		 * Array - if $var is an array and some of $_POST[$key] exist<br/>
		 * NULL - if the key was not set
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function post($var=NULL,$clean=array())
		{
			if ($var === true) return array_keys($_POST);
			if ($var === null) return (isset($_POST)&&count($_POST)>0)?true:false;
			$r = func_get_args();if(!isset($r[2]))$var=$this->sg('post',$var);
			
			if (is_array($var))
			{
				$result = array();
				foreach ($var AS $k=>$v)
				{
					if (!is_null($v) && !is_null($v = $this->post($v,$clean,TRUE)))
					{
						$result[$k] = $v;
					}
				}
				return $result;
			}
			else
			{
				// Maybe just one filter?
				if (is_string($clean)) $clean = array($clean);
				
				// Apply the filters
				if (is_array($clean) && count($clean) > 0)
				{
					foreach ($clean AS $filter)
					{
						if (method_exists($this, 'clean_'.$filter))
						{
							$var = call_user_func_array(array($this, 'clean_'.$filter), array($var));
						}
					}
				}
			}
			
			return $var;		
		}// end function post()
		
		/**
		 * Check wether a file is pending a save through POST
		 * 
		 * @example
		 * # Get all files ($_FILES)
		 * $this->input->file();
		 * # Get the file info for form input named "file1"
		 * $this->input->file('file1'); 
		 * @link http://octoms.com/doc/input_cl/file
		 * 
		 * @method file()
		 * @param string $formInputName
		 * @return array/null <br/>
		 * Returns NULL if the requested form input was not passed.
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function file($formInputName=NULL)
		{
			if (isset($_FILES) && is_array($_FILES))
			{
				if (!empty($formInputName))
				{
					return isset($_FILES[$formInputName])?$_FILES[$formInputName]:null;
				}
				else 
				{
					return (count($_FILES)>0)?$_FILES:null;
				}
			}
			else 
			{
				return null;
			}
			
		}// end function file()		

		/**
		 * Upload a file
		 * 
		 * @example 
		 * # Upload a file from the input named 'file1'
		 * # ex. <form..><input name="file1" type="file"/>
		 * # and save it directly to the root, without changing its name
		 * $this->input->upload('file1',ROOT);
		 * # Same as above but save the file as "doc.png"
		 * $this->input->upload('file1',ROOT,'doc.png');
		 * # Same as above but only allow PNG files
		 * $this->input->upload('file1',ROOT,'doc.png','image/png');
		 * # Same as above but only allow PNG and GIF files
		 * $this->input->upload('file1',ROOT,'doc.png',array('image/png','image/gif'));
		 * # Same as above but limit file size to 2000B
		 * $this->input->upload('file1',ROOT,'doc.png',array('image/png','image/gif'),2000);
		 * # Same as above but limit file size to 500-2000B
		 * $this->input->upload('file1',ROOT,'doc.png',array('image/png','image/gif'),array(500,2000));
		 * # Allow files with the PNG extension
		 * $this->input->upload('file1',ROOT,NULL,NULL,NULL,'png');
		 * # Allow files with the PNG and GIF extensions
		 * $this->input->upload('file1',ROOT,NULL,NULL,NULL,array('png','gif');
		 * @link http://octoms.com/doc/input_cl/upload
		 * 
		 * @method upload()
		 * @param string $formInputName
		 * @param string $folder
		 * @param string $desiredName
		 * @param string/array $allowedTypes
		 * @param string/array $allowedSizes
		 * @param string/array $allowedExt
		 * @param boolean $replaceFile
		 * @return boolean TRUE on success, throws Exception otherwise
		 * @throws Exception
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function upload($formInputName=NULL,$folder=NULL,$desiredName=NULL,$allowedTypes=NULL,$allowedSizes=NULL,$allowedExt=NULL,$replaceFile=FALSE)
		{			
			// The form input name is mandatory
			if (empty($formInputName))
				throw new Exception('Form input not specified.', 0);
				
			// Verify the input
			if (!isset($_FILES[$formInputName]))
				throw new Exception('No file specified for input "'.$formInputName.'".', 1);
			
			// Save the file
			$f = $_FILES[$formInputName];
				
			// Format the folder name
			$folder = trim(str_replace(array(WS,' '),array(DS,'_'),preg_replace('%[^a-zA-Z0-9_/\\\\\s]%i', '', $folder)));
			if (strlen($folder) == 0)
			{
				throw new Exception("Invalid destination folder.", 2);
			}
			
			// The default desired name is the user's local filename
			if (empty($desiredName))
				$desiredName = $f['name'];
			
			// Do not allow dangerous file names
			if (in_array($desiredName, array('.htaccess','php.ini','httpd.conf')))
			{
				throw new Exception('File not allowed.', 3);
			}
				
			// Verify the type
			if (is_array($allowedTypes))
			{
				if (!in_array($f['type'], $allowedTypes))
				{
					throw new Exception('File type ('.$f['type'].') not allowed.', 4);
				}
			}
			elseif (is_string($allowedTypes))
			{
				if (strtolower(trim($f['type'])) != strtolower(trim($allowedTypes)))
				{
					throw new Exception('File type ('.$f['type'].') not allowed.', 4);
				}
			}
			
			// Verify the size
			if (is_array($allowedSizes))
			{
				if ($f['size'] < intval($allowedSizes[0]) || $f['size'] > intval($allowedSizes[1]))
				{
					throw new Exception('File size ('.$f['size'].') not allowed.', 5);
				}
			}
			elseif (is_string($allowedSizes) || is_numeric($allowedSizes))
			{
				if ($f['size'] > intval($allowedSizes))
				{
					throw new Exception('File size ('.$f['size'].') not allowed.', 5);
				}
			}
			
			// Verify the extension
			$ext = strpos($f['name'], '.')===FALSE?'':(substr($f['name'], strrpos($f['name'], '.')+1));
			if (is_array($allowedExt))
			{
				if (!in_array($ext, $allowedExt))
				{
					throw new Exception('File extension "'.$ext.'" not allowed.', 6);
				}
			}
			elseif (is_string($allowedExt))
			{
				if (strtolower(trim($ext)) != strtolower(trim($allowedExt)))
				{
					throw new Exception('File extension "'.$ext.'" not allowed.', 6);
				}
			}
			
			// Verify that no other errors occured
			if ($f['error']>0)
			{
				// http://php.net/manual/en/features.file-upload.errors.php
				$errors = array(
					'No error.',
					'The uploaded file exceeds the upload_max_filesize directive in php.ini.',
					'The uploaded file exceeds the MAX_FILE_SIZE directive that was specified in the HTML form.',
					'The uploaded file was only partially uploaded.',
					'No file was uploaded.',
					'An undocumented error occured.',
					'Missing a temporary folder.',
					'Failed to write file to disk.',
					'A PHP extension stopped the file upload.'
				);
				$message = isset($errors[$f['error']])?$errors[$f['error']]:'An unkwnown error occured.';
				throw new Exception('Return code ('.$message.')', 7);
			}
			
			// Verify that we are not attempting an overwrite
			if (!$replaceFile && file_exists(ROOT.DS.'upload'.DS.$folder.DS.$desiredName))
			{
				throw new Exception('File already exists', 8);
			}
			
			// Create the folder if it does not exist
			if (!is_dir($this->uploadDir.DS.$folder))
			{
				if (!mkdir($this->uploadDir.DS.$folder,0644,true))
				{
					throw new Exception('Could not create directory "'.$folder.'"',	9);
				}
			}
			
			// Upload the file!
			if(!move_uploaded_file($f['tmp_name'], $this->uploadDir.DS.$folder.DS.$desiredName))
			{
				throw new Exception('Could not move the file "'.$f['name'].'" to "'.$folder.'".', 10);
			}
			
			// All done
			return true;
			
		}// end function upload()
	
		/**
		 * File list - Get files from te upload folder
		 * 
		 * @example 
		 * // Get the files from ROOT/UPLOAD_FOLDER/foo
		 * $this->input->file_list('foo');
		 * // Get the files from ROOT/UPLOAD_FOLDER
		 * $this->input->file_list();
		 * @link http://octoms.com/doc/input_cl/file_list
		 * 
		 * @param string $folder
		 * @return array/null - array of array('name'=>string,'size'=>int,'url'=>string) or NULL on failure
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function file_list($folder=null)
		{			
			// The directory
			$dir = rtrim($this->uploadDir.DS.$folder,DS);
			
			// Get all the files in the directory
			if (is_dir($dir))
			{
				$filesFound = array();
				if (FALSE !== $files = scandir($dir))
				{
					foreach ($files AS $file)
					{
						if (is_file($dir.DS.$file))
						{
							$filesFound[] = array(
								'name'	=> $file,
								'size'	=> filesize($dir.DS.$file),
								'url'	=> $this->uploadWeb.WS.str_replace(DS, WS, $folder).WS.urlencode($file)
							);
						}
					}
				}
				// Return the findings
				return $filesFound;
			}
			
			// Nothing found
			return null;
			
		}// end function file_list()
		
		/**
		 * Get a JSON input from a _post variable
		 * 
		 * @example 
		 * # Decode the JSON in $_POST['x']
		 * $data = $this->input->json('x');
		 * # Decode the JSON from a local variable $var
		 * $data = $this->input->json($var,true);
		 * 
		 * @link http://octoms.com/doc/input_cl/json
		 * 
		 * @method json()
		 * @param string $data
		 * @param bool $local
		 * @return mixed - Array or string on success, False if there is no 
		 * post data or Null if the data cannot be decoded
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function json($data,$local=FALSE)
		{
			// Get the json
			if ($local == FALSE && FALSE === ($data = $this->post($data)))
			{
				return $data;
			}
			
			// Try to decode the data
			if (NULL === ($data = json_decode($data)))
			{
				return $data;
			}
			
			// Load the recursion helper
			if (!isset(octoms::$oms_helpers['recursion_ch']))
			{
				require_once CORE_HELP . DS . 'recursion_ch' . EXT;
				octoms::$oms_helpers['recursion_ch'] = TRUE;
			}
			
			// Use arrays
			return object_to_array($data);
			
		}// end function json()
		
		/**
		 * Remove html tags
		 * 
		 * @example 
		 * # Remove all HTML tags except 'a','img' and 'code'
		 * $this->input->clean_tags($input);
		 * # Remove all HTML tags
		 * $this->input->clean_tags($input,TRUE);
		 * # Remove all HTML tags except the 'p'
		 * $this->input->clean_tags($input,array('p'));
		 * @link http://octoms.com/doc/input_cl/clean_tags
		 * 
		 * @param string $input
		 * @param boolean/array $tags
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_tags($input='',$tags=NULL)
		{
			if (is_null($tags))
			{
				return strip_tags($input,'<'.implode('><',array('a','img','code')).'>');
			}
			elseif (is_array($tags))
			{
				return strip_tags($input,'<'.implode('><',$tags).'>');
			}
			else 
			{
				return strip_tags($input);
			}
		}// end function clean_tags()
		
		/**
		 * Remove line feeds and extra whitespaces
		 * 
		 * @example 
		 * $clean = $this->input->clean_whitespaces($input);
		 * @link http://octoms.com/doc/input_cl/clean_whitespaces
		 * 
		 * @param string $input
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_whitespaces($input='')
		{
			// Strip line feeds and excessive tabs, spaces and &nbsp;
			return preg_replace('%[\r\n]+|<br[^>]*/>|(&nbsp;){2,}|[\s]{2,}%i',' ',$input);
			
		}// end function clean_whitespaces()
		
		/**
		 * Remove images but keep alts, if possible
		 * 
		 * @example 
		 * $clean = $this->input->clean_images($input);
		 * @link http://octoms.com/doc/input_cl/clean_images
		 * 
		 * @param string $input
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_images($input='')
		{
			// Preserve the alternative text and anchor
			$input = preg_replace('%(<a[^>]*>)(<img[^>]+alt=")([^"]*)("[^>]*>)(<\/a>)%i', '$1$3$5', $input);
			
			// Preserve the alternative text even if it is not anchored
			$input = preg_replace('%(<img[^>]+alt=")([^"]*)("[^>]*>)%i', '$2', $input);
			
			// Remove all of the other images
			return preg_replace('%<img[^>]*>%i', '', $input);	
			
		}// end function clean_images()
		
		/**
		 * Remove scripts, links, styles and html comments
		 * 
		 * @example 
		 * $clean  = $this->input->clean_scripts($input);
		 * @link http://octoms.com/doc/input_cl/clean_scripts
		 * 
		 * @param string $input
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_scripts($input='')
		{
			return preg_replace('%<script[^>]*>.*?<\/script>|<[\?\%]+[php]?[^>]*[\?\%]?>?|(<link[^>]+rel="[^"]*stylesheet"[^>]*>|style="[^"]*")|<style[^>]*>.*?<\/style>|<!--.*?-->%is', '', $input);
			
		}// end function clean_scripts()
		
		/**
		 * Clean all
		 * 
		 * @example 
		 * # Clean to the bone!
		 * $str = $this->input->clean_all($str);
		 * @link http://octoms.com/doc/input_cl/clean_all
		 * 
		 * @param string $str
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_all($str='')
		{
			$str = $this->clean_tags($str,TRUE);
			$str = $this->clean_whitespaces($str);
			$str = $this->clean_images($str);
			return $this->clean_scripts($str);
			
		}// end function clean_all()
		
		/**
		 * Clean the HTML
		 * 
		 * @example 
		 * # Keep al html tags (like a,div etc.)
		 * $this->input->clean_html($input);
		 * # Remove all html tags
		 * $this->input->clean_html($input,false);
		 * # Remove 'random' tags
		 * $this->input->clean_html($input,array('random'));
		 * # Don't encode quotes
		 * $this->input->clean_html($input,null,ENT_NOQUOTES);
		 * @link http://octoms.com/doc/input_cl/clean_html
		 * 
		 * @param string $input
		 * @param array/boolean $keepTags
		 * @param string $quote - ENT_QUOTES | ENT_COMPAT | ENT_NOQUOTES
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_html($input='',$keepTags=TRUE,$quote=ENT_QUOTES)
		{
			// Preserve HTML tags?
			if (is_array($keepTags))
			{
				$input = strip_tags($input,'<'.implode('><',$keepTags).'>');
			}
			elseif ($keepTags === FALSE)
			{
				$input = strip_tags($input);
			}
			
			// Return the html encoded string
			return htmlentities($input,$quote,'UTF-8');
			
		}// end function clean_html()
		
		/**
		 * Clean an article/post/comment
		 * 
		 * @example 
		 * @link http://octoms.com/doc/input_cl/clean_article
		 * 
		 * @param string $str
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function clean_article($str='')
		{
			// Paragraphs are NL-separated
			$str = preg_replace('/<([^>]*?)>(.*?)<\1>/', '\2'."\n", $str);
			
			// Remove whitespaces
			$str = preg_replace('%(&nbsp;){2,}|[ \t]{2,}%i',' ',$str);
			
			// Find and sanitize all <code> blocks
			$str = preg_replace_callback('%(<code[^>]*>)([^\e]*)</code>%i',create_function('$matches','return "<code>".(octoms(\'input\')->clean_html($matches[2]))."</code>";'),$str);
					
			// Remove unwanted tags
			$str = $this->clean_tags($str);

			// Remove images
			$str = $this->clean_images($str);
			
			// Remove scripts
			$str = $this->clean_scripts($str);
			
			// Return the result
			return trim($str);
			
		}// end function clean_article()
		
		/**
		 * Close open tags
		 * 
		 * @example 
		 * @link http://octoms.com/doc/input_cl/close_tags
		 * 
		 * @param string $html
		 * @return string - tags closed
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function tags_close($html='')
		{
			// Find open tags
			preg_match_all('%<(?!meta|img|br|hr|input|link\b)\b([a-z]+)\b%iU', $html, $o);
		    
			// Find closed tags
			preg_match_all('%</([a-z]+)>%iU', $html, $c);

			// All good?
			if(count($o[1])==count($c[1])) return $html;
		   
			// Close the HTML tags
			foreach (array_reverse($o[1]) AS $k => $v)
			{
				if (!in_array($v, $c[1]))
				{
					$html .= '</'.$v.'>';
				}
				else 
				{
					unset($c[1][array_search($v, $c[1])]);
				}
			}
			
			// Return the result
		    return $html;
		    
		}// end function tags_close()
		
		/**
		 * Remove broken tags
		 * 
		 * @example 
		 * @link http://octoms.com/doc/input_cl/tags_whipe
		 * 
		 * @param string $htmls
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function tags_whipe($html = '')
		{
			return preg_replace('%<[^\b<>]*$%', '', $html);
		}// end function tags_whipe()
		
		/**
		 * Create an abstract
		 * 
		 * @example 
		 * @link http://octoms.com/doc/input_cl/abstr
		 * 
		 * @param string $html
		 * @param int $legth - number of words
		 * @return string
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function abstr( $string, $len=500)
		{
			// Clean the whitespaces
			$string = $this->clean_whitespaces($string);
			
			// Any trimming to be done?
			if (strlen($string) > $len)
			{
				$pos = ($len - stripos(strrev(substr($string, 0, $len)), ' '));
				$sum = substr($string, 0, $pos-1);
				$chr = $sum[strlen($sum)-1];
				if (strpos($chr, '.,!?;:'))
				{
					$sum = substr($sum, 0, strlen($sum)-1);
				}
				return $this->tags_close($this->tags_whipe($sum.'&#8230;'));
			}
			else
			{
				return $string;
			}
		}// end function abstr()
		
	}// end class input_cl

/* End Of File <input.inc> */