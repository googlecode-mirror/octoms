<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/*
 * @package       Fervoare.com
 * @link          http://fervoare.com
 * @copyright     Copyright 2012, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         Fervoare.com 0.0.1
 */
	
	/*
	 * CSV library
	 * 
	 * 
	 * @package Fervoare.com
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class csv_cl
	{		
		
		/*
		 * Transform an array object to a CSV text
		 * 
		 * @param array $array
		 * @return string - CSV formatted data
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		protected function _arrayToCsv($array=null)
		{
			// Verify there is data to encrypt
			if (is_array($array) && count($array)>0)
			{
				// Prepare the result string
				$result = "";
				
				// Process each row
				foreach ($array AS $row => $columns)
				{
					if (is_array($columns) && count($columns)>0)
					{
						foreach ($columns AS $k => $col)
						{
							// Boolean or null?
							if (is_bool($col) || is_null($col))
							{
								if (is_bool($col))
								{
									$col = $col?'true':'false';
								}
								else
								{
									$col = '';
								}
							}
							// Other data type
							else
							{
								// Not a string or a numeric value?
								if (!is_string($col) && !is_numeric($col))
								{
									// To String
									$col = var_export($col,true);
								}
								
								// No new lines
								$col = str_replace(array("\r\n","\r","\n"),"", $col);
								
								// CSV string encryption rules must be followed
								if (preg_match('/^.*[^a-zA-Z0-9.]+.*$/i', $col))
								{
									// Escape double quotes
									$col = sprintf('"%s"',preg_replace('/"/i', '""', $col));
								}
							}
							
							// Replace the column
							$columns[$k] = $col;
						}
						
						// Append the result
						$result .= implode(",", $columns) . "\r\n";
					}
					else 
					{
						// An empty row
						$result .= "\r\n";
					}
				}
				
				// Return the result
				return $result;
			}
			else
			{
				return "";
			}
		}// end function _arrayToCsv()

		/*
		 * CSV text to array
		 * 
		 * @param string $csv
		 * @return array
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		protected function _csvToArray($csv=null)
		{
			// Return an empty array if the provided data is empty
			if(is_null($csv))
			{
				return array();
			}
			
			// Break the contents into lines
			$csv = explode("\r\n", $csv);
			
			// Break each line into an array of columns
			foreach($csv AS $row => $line)
			{
				if (!is_null($line))
				{
					// Get the columns
					@preg_match_all('/(?:^|,)?("(?:[^"]+|"")*"|[^,]*)/', $line, $regs);
					$line = isset($regs[1])?$regs[1]:array();

					// Format each cell
					foreach($line AS $k=>$v)
					{
						if (!is_null($v))
						{
							// Remove outside double quotes
							if (preg_match('/^"(.*)"$/i', $v)) 
							{
								$v = preg_replace('/^"(.*)"$/i', '$1', $v);
							}
							
							// Remove the double quotes escaping
							if (preg_match('/"{2}/i', $v)) 
							{
								$v = preg_replace('/"{2}/i', '"', $v);
							}
							
							// Done
							$line[$k] = $v;
						}
					}
					
					// Update the row
					$csv[$row] = $line;
				}
			}
			
			// Return the array
			return $csv;
			
		}// end function _csvToArray()
		
		/*
		 * Prevent use of functions directly from the parent class
		 * 
		 * @param string $method
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		protected function _use($method=null)
		{
			if (!is_null($method) && !isset($this->address) && !isset($this->csv))
			{
				throw new Exception('Call the '.$method.'() method from an object created with {csv}->open().');
			}
		}// end function _use()
		
		/**
		 * Append a row to a CSV file
		 * 
		 * @example 
		 * // Insert a new row directly to a CSV file
		 * {csv}->insert(array('a','b','c'),'path/to/file.csv');
		 * // Insert multiple rows directly to a CSV file
		 * {csv}->insert(array(array('a','b','c'),array('foo','bar','baz')),'path/to/file.csv');
		 * 
		 * @param array $row
		 * @param string $file
		 * @return $this
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function insert($row=null,$file=NULL)
		{
			if (is_array($row))
			{
				// Open the file to append the row
				if(FALSE !== $fh = @fopen($file, 'a'))
				{
					// Write the new line
					if(FALSE === @fwrite($fh, $r = $this->_arrayToCsv(array_depth($row)>1?$row:array($row))))
					{
						throw new Exception('Could not write to CSV file "'.$file.'".');
					}
					
					// Close the file
					fclose($fh);

					// All done
					return $this;
				}
				else 
				{
					throw new Exception('Could not open CSV file "'.$file.'" (append mode).');
				}
			}
			else
			{
				throw new Exception('The first argument must be an array.');
			}
		}// end function insert()
		
		/**
		 * Open a CSV file
		 * 
		 * @example
		 * // Open a CSV file
		 * {csv}->open('path/to/file.csv');
		 * 
		 * @param string $file
		 * @return csv_cl_i
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function open($file=null)
		{
			if (file_exists($file))
			{
				// Open the file
				if(FALSE !== $fh = @fopen($file, 'r'))
				{
					// Read the contents
					if(FALSE === $fc = @fread($fh, filesize($file)))
					{
						throw new Exception('Could not read CSV file "'.$file.'".');
					}
					
					// Close the file
					fclose($fh);
					
					// Return a new CSV object
					return new csv_cl_i($this->_csvToArray($fc),$file);
				}
				else
				{
					throw new Exception('Could not open CSV file "'.$file.'" (read mode).');
				}
			}
			else
			{
				throw new Exception('CSV file "'.$file.'" not found.');
			}
		}// end function open()
		
		/**
		 * Empty the contents of a row
		 * 
		 * @example 
		 * // Empty the first row
		 * {csv_cl_i}->rowEmpty(0);
		 * 
		 * @param int $rowKey
		 * @return csv_cl_i
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function rowEmpty($rowKey=null)
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Unset the key
			isset($this->csv[intval($rowKey)])?($this->csv[intval($rowKey)]=null):null;
			
			// Done
			return $this;
			
		}// end function rowEmpty()
		
		/**
		 * Read a row
		 * 
		 * @example 
		 * // Get the first row as an array
		 * {csv_cl_i}->rowGet(0);
		 * // Get all the rows
		 * {csv_cl_i}->rowGet();
		 * 
		 * @param int $rowKey
		 * @return array - the required row or the entire CSV array, if $rowKey not set
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function rowGet($rowKey=NULL)
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Return the row by key
			return isset($this->csv[$rowKey])?$this->csv[$rowKey]:$this->csv;
			
		}// end function rowGet()
		
		/**
		 * Insert a row in the CSV array
		 * 
		 * @example 
		 * // Insert a row at the bottom of the array
		 * {csv_cl_i}->rowInsert(array('a','b','c'));
		 * // Insert a row at the top of the array
		 * {csv_cl_i}->rowInsert(array('a','b','c'),true);
		 * // Insert an empty row at the bottom of the array
		 * {csv_cl_i}->rowInsert();
		 * 
		 * @param array $row
		 * @return csv_cl_i
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function rowInsert($row=NULL,$insertToTop=FALSE)
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Append the element
			$insertToTop?array_unshift($this->csv, $row):array_push($this->csv, $row);
			
			// All done
			return $this;
			
		}// end function rowInsert()
		
		/**
		 * Remove a row completely
		 * 
		 * @example 
		 * // Delete row #2
		 * {csv_cl_i}->rowShift(2);
		 * // Delete row #0
		 * {csv_cl_i}->rowShift();
		 * 
		 * @param int $rowKey
		 * @return csv_cl_i
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function rowShift($rowKey=null)
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Unset the key
			unset($this->csv[intval($rowKey)]);
			
			// Done
			return $this;
			
		}// end function rowShift()
		
		/**
		 * Update a row
		 * 
		 * @example 
		 * // Update row #2
		 * {csv_cl_i}->rowUpdate(array('a','b','c'),2);
		 * // Update row #0
		 * {csv_cl_i}->rowUpdate(array('foo','bar','baz'));
		 * 
		 * @param array $rowValue
		 * @param int $rowKey
		 * @return csv_cl_i
		 * @throws Exception
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function rowUpdate($rowValue=null,$rowKey=null)
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Update the element
			$this->csv[intval($rowKey)] = $rowValue;
			
			// All done
			return $this;
			
		}// end function rowUpdate()
		
		/**
		 * Save the work on the current CSV file
		 * 
		 * @example 
		 * // Save the work to the CSV file accessed with the open() method
		 * // before emptying the row #2
		 * {csv_cl_i}->open('path/to/file.csv')->rowEmpty(2)->save();
		 * 
		 * @return csv_cl_i
		 * @package Fervoare.com
		 * @throws Exception
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function save()
		{
			// Allow access to this method from the child instance only
			$this->_use(__FUNCTION__);
			
			// Open the file
			if (FALSE !== $fh = @fopen($this->address,'w'))
			{
				// Replace the content
				if (FALSE === @fwrite($fh, $this->_arrayToCsv($this->csv)))
				{
					throw new Exception('Could not write to CSV file "'.$this->address.'".');
				}
				
				// Close the file handler
				fclose($fh);
				
				// All done
				return $this;
			}
			else
			{
				throw new Exception('Could not open CSV file "'.$this->address.'" (write mode).');
			}
		}// end function save()
		
	}// end class csv_cl
	
	/*
	 * CSV instances
	 * 
	 * 
	 * @package Fervoare.com
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class csv_cl_i extends csv_cl
	{
		/**
		 * The CSV file's address
		 * 
		 * @var string
		 */
		public $address;
		
		/**
		 * The CSV array object
		 *
		 * @var array
		 */
		public $csv;
		
		/*
		 * Class constructor
		 * 
		 * @param array $csv - csv contents
		 * @param string $address - file location
		 * @return csv_cl_i
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function __construct($csv=null,$address=null)
		{
			$this->csv = $csv;
			$this->address = $address;
		}// end function __construct()
		
		/*
		 * We don't need to open another CSV file here
		 * 
		 * @return csv_cl_i
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function open()
		{
			return $this;
		}// end function open()
		
	}// end class csv_cl_i
	
/* End Of File <csv.inc> */