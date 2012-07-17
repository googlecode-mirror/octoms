<?php
/**
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2011, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
 * 
 * A copy of the CC BY-SA 3.0 is provided along with this software 
 * (see license.txt, valid if its md5 hash is "29f674e594bb0477f60ae9d45ea30a61").
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */

	/**
	 * Basic application information
	 */
	define('OCTOMS', '0.0.1');			# the framework version
	define('ROOT', dirname(__FILE__));	# the framework root
	define('APP_FOLDER', 'app');		# the application folder name (you may change this)

	/**
	 * Load the rest of the constants
	 */
	require_once ROOT.DIRECTORY_SEPARATOR.'core'.DIRECTORY_SEPARATOR.'config'.DIRECTORY_SEPARATOR.'constants.inc';
	
	/**
	 * Load the bootstrap utility
	 */
	require_once CORE_LIB.DS.'octoms'.EXT;
	
	/**
	 * It's the new OctoMS!
	 */
	new octoms;
	
/* End Of File <index.php> */