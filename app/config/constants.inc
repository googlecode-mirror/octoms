<?php if(!defined('OCTOMS')){header('HTTP/1.1 403');die('{"error":"forbidden"}');}
/**
 * @package       OctoMS
 * @link          http://octoms.com
 * @copyright     Copyright 2011, Valentino-Jivko Radosavlevici (http://valentino.radosavlevici.com)
 * @license       GPL v3.0 (http://www.gnu.org/licenses/gpl-3.0.txt)
 * 
 * Redistributions of files must retain the above copyright notice.
 * 
 * @since         OctoMS 0.0.1
 */
	
	/**
	 * Set the environment
	 */
	define('ENVIRONMENT', 0);	# 0 - development; 1 - production
	
	/**
	 * Display the error wizard?
	 */
	define('ERROR_WIZARD', 0);	# 1 - yes; 0 - no

	/**
	 * Global Encryption string for the blowfish algorythm (56 characters in length)
	 */
	define('BF_KEY', 'SADFo92jzVnzSj39IUYGvi7eL8v6RvJH9Cytuiouh347vCytdyUFl123');
	
	/**
	 * Global Encryption salt for the blowfish algorythm (8 characters in length)
	 */
	define('BF_SALT', '92jzVnzS');
	
	/**
	 * API Octoms Encryption string (56 characters in length)
	 * If you changed this key make sure to log into your account at http://octoms.com
	 * and change it in your settings also.
	 */
	define('API_KEY', 'SADFo92jzVnzSj39abcGvi6eL8v6RvJH8C123iouh547vCytdyUFl76R');
	
	/**
	 * API Octoms Project id
	 * TODO link to demo page
	 */
	define('API_PID', '1');
	
	/**
	 * API Octoms User e-mail
	 * TODO link to demo page
	 */
	define('API_UID', 'mail@address.com');
	
	/**
	 * API - Individual access token
	 */
	define('API_TKN', '1345trew');
	
	/**
	 * Site information
	 */
	define('DOMAIN', false!==strpos($_SERVER['HTTP_HOST'], ':')?substr($_SERVER['HTTP_HOST'],0,strpos($_SERVER['HTTP_HOST'], ':')):$_SERVER['HTTP_HOST']);	# the domain name
	define('DOMAIN_PROTOCOL', 'http://');		# 'http://' or 'https://'
	
	/**
	 * Define the cookie parameters
	 */
	define('COOKIE_NAME', 'octoms');		# the cookie name; default: 'octoms'
	define('COOKIE_EXPIRE', 2592000);		# cookie expiration time; default: 30 days from now
	define('COOKIE_PATH', '/');				# cookie path; default: "/"
	define('COOKIE_DOMAIN', '.'.DOMAIN);	# cookie domain; for sitewide cookie set .domain.com; default: '.'.DOMAIN
	
	/**
	 * Define the default stage, controller and method
	 */
	define('DEF_STAGE', 'test');	# stage name
	define('DEF_CONTR', 'test');	# controller name
	define('DEF_METHO', 'index');	# methode name
	
	/**
	 * Define the database connection credentials
	 */
	define('DB_SERV', 'localhost');	# server
	define('DB_USER', 'root');			# username
	define('DB_PASS', '');				# password
	define('DB_DATA', 'octoms');		# database
	define('DB_PREF', '');				# table prefix
	
	/**
	 * Fervoare CSM constants
	 */
	define('FERV_VERSION','1.0.0'); # Fervoare CMS version
	define('FERV_ADDONS', STAGES.DS.'addons');	#addons folder
	define('FERV_WIDGETS', STAGES.DS.'widgets'); #widgets folder
	define('FERV_TEMPLATE', 'default');	#the template
	
	
/* End Of File <constants.inc> */