<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN" "http://www.w3.org/TR/html4/loose.dtd">
<html>
	<head>
		<title>OctoMS Help</title>
		<link rel="stylesheet" type="text/css" href="<?php echo WIZARD;?>/css/style.css">
		<link rel="icon" type="image/ico" href="<?php echo WIZARD;?>/img/favicon.ico">
		
		<meta name="Copyright" content="OctoMS - www.octoms.com">
		<meta name="Author" content="Valentino-Jivko Radosavlevici - valentino.radosavlevici.com">
		<meta name="Robots" content="no-index">
		<meta http-equiv="Content-Type" content="text/html; charset=utf-8">
		
		<script type="text/javascript" src="<?php echo WIZARD;?>/js/common/jquery.js"></script>
		<script type="text/javascript" src="<?php echo WIZARD;?>/js/common/jquery-ui.js"></script>
		<script type="text/javascript" src="<?php echo WIZARD;?>/js/functions.js"></script>
		<script type="text/javascript">
			API_URL = '<?php echo WEBROOT . '/~help/wizard/'; ?>';
			SEARCH = '<?php echo $info->s;?>';
			DEBUG = '<?php echo $info->d;?>';
			ERROR = <?php echo is_null($info->e)?'""':$info->e;?>;
			EXCEPTION = <?php echo is_null($info->x)?'""':$info->x;?>;
			EXPORT = <?php echo is_null($info->c)?'""':$info->c;?>;
		</script>
	</head>
	<body>
		<div class="noscript">
			<h1>Ooops...</h1><br>
			<p>It appears that your web browser does not support JavaScript, or you have temporarily disabled scripting. Either way, this site won't work without it. <br>Please re-enable JavaScript and refresh this page to access the <span class="octoms">OctoMS</span> helper. </p>
		</div>
		<div id="header">
			<div id="header-content">
				<div id="top-strip">
					<div id="title">
						<div class="logo">&nbsp;</div><span class="octoms"><a target="_blank" id="logo_anchor" href="http://octoms.com">OctoMS</a></span>
						<span><a id="license" target="_blank" href="http://creativecommons.org/licenses/by-sa/3.0/">&nbsp;</a></span>
					</div>
				</div>
				<div id="boxes">
					<div id="box-left">
						<div class="spacer"></div><div class="l"></div><input type="text" id="search" value="Search..."><div class="r"></div><div id="input-search"></div>
					</div>
					<div id="box-right">
						<div class="divider"></div><div class="l"></div><input type="text" id="debug" value="Debug..."><div class="r"></div><div id="input-debug"></div><div class="spacer"></div>
					</div>
				</div>
			</div>
		</div>
		<div id="wrapper">
			<div id="left-bar"></div>
			
			<div id="left"><div class="scrollbar"><div class="track"><div class="thumb"><div class="end tip1"></div><div class="end tip2"></div></div></div></div>
				<div class="viewport">
					<div class="overview">
						<div class="full-content"></div>
					</div>
				</div>
			</div>
			<div id="right"><div class="scrollbar"><div class="track"><div class="thumb"><div class="end tip1"></div><div class="end tip2"></div></div></div></div>
				<div class="viewport">
					<div class="overview">
						<div class="full-content"></div>
					</div>
				</div>
			</div>

			<div id="drag-bar"><div id="drag-bar-thumb"></div></div>
			<div id="right-bar"></div>
		</div>
		<div id="footer">
			<div id="bottom-bar"></div>
		</div>
	</body>
</html>