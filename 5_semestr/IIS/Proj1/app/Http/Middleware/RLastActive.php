<?php
/**
 * Created by PhpStorm.
 * User: Holajz
 * Date: 04.12.2017
 * Time: 16:33
 */

namespace App\Http\Middleware;

use Closure;
use Illuminate\Support\Facades\Session;

class RLastActive
{
    public function handle($request, Closure $next)
    {
        Session::put('lastActive', date('U'));
        Session::forget('idleWarningDisplayed');
        Session::forget('logoutWarningDisplayed');

        return $next($request);
    }
}