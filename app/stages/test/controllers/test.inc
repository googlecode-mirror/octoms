<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/*
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2012, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */
	
	/*
	 * Test class
	 * 
	 * @link http://octoms.com/doc/test
	 * 
	 * @package OctoMS
	 * @subpackage test
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class test extends octoms
	{
		
		/**
		 * Class constructor
		 * 
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function index()
		{
			print 'Hello world!';
			
		}// end function index()
		
	}// end class test
	
	
/* End Of File <test.inc> */