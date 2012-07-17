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
	 * Session management; 0731 458 795
	 * 
	 * @link http://octoms.com/doc/session
	 * 
	 * @package OctoMS
	 * @subpackage session
	 * @version 0.1
	 * 
	 * @author Valentino-Jivko Radosavlevici
	 */
	class session_cl
	{
		
		/**
		 * The current session Id
		 * @var string
		 */
		var $id;
		
		/**
		 * Class constructor
		 * 
		 * @link http://octoms.com/doc/session/__construct
		 * 
		 * @param 
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function __construct()
		{
			// Don't store the session Id in plain sight, encode it in our cookie instead
			@ini_set('session.use_cookies',0);
			
		}// end function __construct()
		
		/**
		 * Check if the session was started
		 * 
		 * @example $this->session->is_started();
		 * 
		 * @return boolean - True if the session was started already
		 * @package Fervoare.com
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function is_started()
		{
			return session_id() != '';	
		}// end function is_started()
		
		/**
		 * Start the session; prevent cookie stealing and session sniffing
		 * 
		 * @example $this->session->start();
		 * @link http://octoms.com/doc/session/start
		 * 
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function start()
		{	
			// First, get the cookie
			$cookie = $this->cookie_get();

			// Invalid cookie set?
			empty($cookie)?$this->cookie_reset():$cookie=(array)$cookie;

			// Set an empty user agent
			if (!isset($_SERVER['HTTP_USER_AGENT'])) $_SERVER['HTTP_USER_AGENT'] = '';
			
			// Verify the user's cookie x-verify credentials
			if (isset($cookie['x-verify']) && $cookie['x-verify']->ua == md5($_SERVER['HTTP_USER_AGENT']) && $cookie['x-verify']->ip == ip2long($_SERVER['REMOTE_ADDR']))
			{
				// The user passed; restore the session
				$this->id=$cookie['x-verify']->sessid;

				// Don't continue if the session was already started
				if (session_id()==$this->id) return $this;
			}
			else 
			{
				// Destroy the session(s)
				session_start();
				session_unset();
				session_destroy();
				
				// Regenerate the session ID
				$this->id = md5(uniqid(mt_rand(),TRUE));
				
				// Store it to the cookie
				$this->cookie_set(
					array('x-verify'=>
						array(
							'sessid'=>$this->id,
							'ua'=>md5($_SERVER['HTTP_USER_AGENT']),
							'ip'=>ip2long($_SERVER['REMOTE_ADDR'])
						)
					)
				);
			}
			
			// Launch the session with the generated Id
			session_id($this->id);
			session_start();
			return $this;
			
		}// end function start()
		
		/**
		 * Destroy the current session
		 * 
		 * @example $this->session->destroy();
		 * @link http://octoms.com/doc/session_cl/destroy
		 * 
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function destroy()
		{
			// Clear all session data
			unset($_SESSION);
			
			// Remove the cookie
			$this->cookie_reset();
			
		}// end function destroy()
		
		/**
		 * Set a session value
		 * 
		 * @example 
		 * // Set key=>value
		 * $this->session->set('key','value');
		 * // Set k1=>v1, k2=>v2
		 * $this->session->set(array('k1'=>'v1','k2'=>'v2'));
		 * 
		 * @link http://octoms.com/doc/Session/set
		 * 
		 * @param $key and $value or $array
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function set()
		{
			$params = func_get_args();
			if (isset($params[0]) && is_array($params[0]))
			{
				foreach ($params AS $k=>$v)
				{
					$_SESSION[$k] = $v;
				}
			}
			elseif (isset($params[0]) && isset($params[1]))
			{
				$_SESSION[$params[0]] = $params[1];
			}
			
			return $this;
		}// end function set()
		
		/**
		 * Get a session value by key
		 * 
		 * @example $this->session->get('key');
		 * 	$this->session->get(array('key1','key2'));
		 * @link http://octoms.com/doc/Session/get
		 * 
		 * @param string/array $key(s)
		 * @return mixed
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function get($keys=NULL)
		{
			if (is_array($keys))
			{
				$result = array();
				foreach ($keys AS $k)
				{
					$result[$k] = isset($_SESSION[$k])?$_SESSION[$k]:NULL;
				}
				return $result;
			}
			elseif (is_string($keys))
			{
				return isset($_SESSION[$keys])?$_SESSION[$keys]:NULL;
			}
			
			return NULL;
		}// end function get()
		
		/**
		 * Unset a sessions value by key
		 * 
		 * @example $this->session->reset('key');
		 * 	$this->session->reset(array('key1','key2'));
		 * 	$this->session->reset(); # clean all session values
		 * @link http://octoms.com/doc/Session/reset
		 * 
		 * @param string/array $key(s)
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function reset($keys=NULL)
		{
			if (is_array($keys))
			{
				foreach($keys AS $k)
				{
					unset($_SESSION[$k]);
				}
			}
			elseif (is_string($keys))
			{
				unset($_SESSION[$keys]);
			}
			else
			{
				unset($_SESSION);
			}
			
			return $this;
			
		}// end function reset()
		
		/**
		 * Set a flash variable or a set of flash variables
		 * 
		 * @example $this->session->flash_set('key','value');
		 * 	$this->session->flash_set(array('val1','val2'));
		 * @link http://octoms.com/doc/Session/flash_set
		 * 
		 * @param $key and $value or $array
		 * @return session_cl 
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function flash_set()
		{
			$params = func_get_args();
			if (isset($params[0]) && is_array($params[0]))
			{
				foreach ($params[0] AS $k => $v)
				{
					$_SESSION['__flash__'][$k] = $v;
				}
			}
			elseif (isset($params[0]) && isset($params[1]))
			{
				$_SESSION['__flash__'][$params[0]] = $params[1];
			}
			
			return $this;
		}// end function flash_set()
		
		/**
		 * Get a flash value (and, obviously, unset it)
		 * 
		 * @example $this->session->flash_get('key');
		 * 	$this->session->flash_get(array('key1','key2'));
		 * @link http://octoms.com/doc/Session/flash_get
		 * 
		 * @param string/array $key(s)
		 * @return mixed
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function flash_get($keys=NULL)
		{
			if (is_array($keys))
			{
				$result = array();
				foreach ($keys AS $k)
				{
					$result[$k] = isset($_SESSION['__flash__'][$k])?$_SESSION['__flash__'][$k]:NULL;
					unset($_SESSION['__flash__'][$k]);
				}
				return $result;
			}
			elseif (is_string($keys))
			{
				if (isset($_SESSION['__flash__'][$keys])){
					$temp = $_SESSION['__flash__'][$keys];
					unset($_SESSION['__flash__'][$keys]);
					return $temp;
				}
				else 
				{
					return NULL;
				}
			}
			
			return NULL;
			
		}// end function flash_get()
		
		/**
		 * Flash reset
		 * 
		 * @example $this->session->flash_reset('key');
		 * 	$this->session->flash_reset(array('key1','key2'));
		 * 	$this->session->flash_reset(); # unset all flash data
		 * @link http://octoms.com/doc/Session/flash_reset
		 * 
		 * @param string/array $keys
		 * @return session_cl
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function flash_reset($keys=NULL)
		{
			if (!empty($keys))
			{
				$this->flash_get($keys);
			}
			else 
			{
				unset($_SESSION['__flash__']);
			}
			
			return $this;
			
		}// end function flash_reset()
		
		/**
		 * Set the cookie (send it to the browser)
		 * 
		 * @example $this->session->cookie_set(array('a','b','c'));
		 * 	$this->session->cookie_set('value','cookieName',-100); #date from the past to kill the cookie 'cookieName'
		 * @link http://octoms.com/doc/Session/cookie_set
		 * 
		 * @param mixed $value
		 * @param string $name
		 * @param int $expire
		 * @return boolean
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function cookie_set($value,$name=COOKIE_NAME,$expire=COOKIE_EXPIRE)
		{
			
			// Set the cookie if the value is not empty
			return empty($value)?FALSE:setcookie($name,blowfish_encrypt(json_encode($value)),(time()+$expire),COOKIE_PATH,(FALSE===ip2long($_SERVER['HTTP_HOST'])?COOKIE_DOMAIN:null));
			
		}// end function cookie_set()
		
		/**
		 * Get the cookie values
		 * 
		 * @example 
		 * @link http://octoms.com/doc/Session/cookie_get
		 * 
		 * @param string $name
		 * @return cookie contents/boolean false
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function cookie_get($name=COOKIE_NAME)
		{
			// Return available cookie data
			return isset($_COOKIE[$name])?json_decode(blowfish_decrypt($_COOKIE[$name])):FALSE;			
		
		}// end function cookie_get()
		
		/**
		 * Reset the cookie (delete it)
		 * 
		 * @example 
		 * @link http://octoms.com/doc/Session/cookie_reset
		 * 
		 * @param string $name
		 * 
		 * @author Valentino-Jivko Radosavlevici
		 */
		function cookie_reset($name=COOKIE_NAME)
		{
			// This cookie has expired
			if(isset($_COOKIE[$name]))
			{
				setcookie($name,'',time()-3600);
				unset($_COOKIE[$name]);
			}
			
		}// end function cookie_reset()
		
	}// end class session_cl

	
/* End Of File <session.inc> */